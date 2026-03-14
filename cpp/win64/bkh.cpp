/*
 * Bitcoin Key Hunter
 * Copyright (C) 2026 Francisco Fuertes García
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */
/*---------------------------------------------------------
# SCRIPT: Bitcoin Key Hunter (bkh)
#     Buscador Multihilo Claves Bitcoin
# VERSIÓN: 5.0
#
# REQUISITO EXTERNO:
# A partir del fichero externo 'objetivos.txt' debemos crear el fichero 'objetivos_hash160.txt'.
# Carga de objetivos desde archivo externo 'objetivos_hash160.txt' con identidad real (Hash160).
# Ejemplo: en 'objetivos.txt' estará '1A1zP1eP5QGefi2DMPTfTL5SLmv7DivfNa' y en 'objetivos_hash160.txt' será '62e907b15cbf27d5425399ebf6f0fb50ebb88f18'.
#
# DIRECCIONES OBJETIVO VÁLIDAS:
# - Direcciones Legacy (empiezan por 1 y tienen entre 26 y 35 caracteres)
# - Direcciones SegWit (empiezan por bc1q y tienen exactamente 42 caracteres)
---------------------------------------------------------*/

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>
#include <chrono>
#include <omp.h>
#ifdef _WIN32
    #include <secp256k1/secp256k1.h>
#else
    #include <secp256k1.h>
#endif
#include <openssl/sha.h>
#include <openssl/ripemd.h>
#include <cstring>
#include <algorithm>
#include <random>
#include <csignal>
#include <limits>

using namespace std;

// --- VARIABLES GLOBALES ---
bool found = false;
bool keep_running = true;
unsigned long long falsos_positivos = 0;

const uint64_t BLOOM_SIZE = 2147483648ULL; 
vector<bool> bloom_filter(BLOOM_SIZE, false);
vector<string> lista_objetivos_confirmados;

void signal_handler(int signum) {
    keep_running = false;
    cout << "\n[!] Deteniendo búsqueda... Finalizando ciclos." << endl;
}

void add_to_bloom(const unsigned char* h) {
    uint64_t* p = (uint64_t*)h;
    bloom_filter[p[0] % BLOOM_SIZE] = true;
    bloom_filter[p[1] % BLOOM_SIZE] = true;
    bloom_filter[(p[0] ^ p[1]) % BLOOM_SIZE] = true;
    bloom_filter[(p[0] + p[1]) % BLOOM_SIZE] = true;
}

bool check_bloom(const unsigned char* h) {
    uint64_t* p = (uint64_t*)h;
    if (!bloom_filter[p[0] % BLOOM_SIZE]) return false;
    if (!bloom_filter[p[1] % BLOOM_SIZE]) return false;
    if (!bloom_filter[(p[0] ^ p[1]) % BLOOM_SIZE]) return false;
    if (!bloom_filter[(p[0] + p[1]) % BLOOM_SIZE]) return false;
    return true;
}

void hex_to_bytes(string hex, unsigned char* bytes) {
    for (unsigned int i = 0; i < 20; i++) {
        bytes[i] = (unsigned char) strtol(hex.substr(i * 2, 2).c_str(), NULL, 16);
    }
}

static const char* ALPHABET = "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz";
string base58_encode(const unsigned char* data, size_t len) {
    vector<unsigned char> digits(len * 137 / 100 + 1, 0);
    size_t digits_len = 1;
    for (size_t i = 0; i < len; i++) {
        unsigned int carry = data[i];
        for (size_t j = 0; j < digits_len; j++) {
            carry += (unsigned int)digits[j] << 8;
            digits[j] = carry % 58;
            carry /= 58;
        }
        while (carry) { digits[digits_len++] = carry % 58; carry /= 58; }
    }
    string res = "";
    for (size_t i = 0; i < len && data[i] == 0; i++) res += '1';
    for (size_t i = 0; i < digits_len; i++) res += ALPHABET[digits[digits_len - 1 - i]];
    return res;
}

string to_legacy_addr(const unsigned char* h160) {
    unsigned char d[25]; d[0] = 0x00; memcpy(d+1, h160, 20);
    unsigned char s1[32], s2[32];
    SHA256(d, 21, s1); SHA256(s1, 32, s2);
    memcpy(d+21, s2, 4);
    return base58_encode(d, 25);
}

string to_bech32_addr(const unsigned char* h160) {
    static const char* charset = "qpzry9x8gf2tvdw0s3jn54khce6mua7l";
    string res = "bc1q";
    vector<int> data; uint32_t acc = 0, bits = 0;
    for (int i = 0; i < 20; ++i) {
        acc = (acc << 8) | h160[i]; bits += 8;
        while (bits >= 5) { bits -= 5; data.push_back((acc >> bits) & 31); }
    }
    for (int v : data) res += charset[v];
    return res;
}

string to_wif(const unsigned char* priv_key, bool compressed) {
    unsigned char data[38]; data[0] = 0x80; memcpy(data + 1, priv_key, 32);
    size_t len = compressed ? 34 : 33; if (compressed) data[33] = 0x01;
    unsigned char sha[32], sha2[32];
    SHA256(data, len, sha); SHA256(sha, 32, sha2);
    memcpy(data + len, sha2, 4);
    return base58_encode(data, len + 4);
}

string to_hex(const unsigned char* data, size_t len) {
    stringstream ss;
    for(size_t i = 0; i < len; ++i) ss << hex << setw(2) << setfill('0') << (int)data[i];
    return ss.str();
}

string get_current_time() {
    time_t now = time(0);
    struct tm tstruct = *localtime(&now);
    char buf[80]; strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);
    return string(buf);
}

int main(int argc, char* argv[]) {
    signal(SIGINT, signal_handler);
    int n_restar = 0;
    string file_path = "objetivos_hash160.txt"; 

    for (int i = 1; i < argc; i++) {
        string arg = argv[i];
        if (arg.substr(0, 2) == "-n") n_restar = atoi(argv[i] + 2);
        else if (arg.substr(0, 2) == "-f") {
            file_path = arg.substr(2);
            file_path.erase(remove(file_path.begin(), file_path.end(), '\"'), file_path.end());
        }
    }

    cout << "=================================================" << endl;
    cout << " BITCOIN KEY HUNTER v5.0 | EXECUTION" << endl;
    cout << "=================================================" << endl;

    string input;
    cout << "[?] Ruta del fichero [" << file_path << "]: ";
    getline(cin, input);
    if (!input.empty()) file_path = input;

    cout << "[?] Hilos a LIBERAR [" << n_restar << "]: ";
    getline(cin, input);
    if (!input.empty()) n_restar = stoi(input);

    ifstream file(file_path);
    if (!file.is_open()) { cout << "[!] ERROR: No se pudo abrir: " << file_path << endl; return 1; }

    cout << "[+] Cargando objetivos en RAM..." << endl;
    string line;
    while (getline(file, line)) {
        line.erase(remove(line.begin(), line.end(), '\r'), line.end());
        line.erase(remove(line.begin(), line.end(), '\n'), line.end());
        if(line.length() == 40) {
            unsigned char b[20];
            hex_to_bytes(line, b);
            add_to_bloom(b);
            lista_objetivos_confirmados.push_back(line);
        }
    }
    file.close();
    sort(lista_objetivos_confirmados.begin(), lista_objetivos_confirmados.end());
    
    int hilos_uso = max(1, omp_get_num_procs() - n_restar);
    omp_set_num_threads(hilos_uso);

    cout << "[+] Registros en RAM: " << lista_objetivos_confirmados.size() << endl;
    cout << "[+] Hilos activos: " << hilos_uso << endl;
    cout << "=================================================\n" << endl;

    unsigned long long total_keys = 0;
    auto start_time = chrono::high_resolution_clock::now();

    #pragma omp parallel
    {
        secp256k1_context* ctx = secp256k1_context_create(SECP256K1_CONTEXT_SIGN);
        unsigned char priv[32], h160_c[20], h160_u[20], pub_c[33], pub_u[65], sha[32];
        mt19937_64 engine(omp_get_thread_num() + (unsigned int)time(0));
        uniform_int_distribution<uint64_t> dist(0, 0xFFFFFFFFFFFFFFFFULL);

        while (keep_running && !found) {
            for(int i=0; i<4; i++) { uint64_t r = dist(engine); memcpy(priv + (i*8), &r, 8); }

            secp256k1_pubkey pubkey;
            if (secp256k1_ec_pubkey_create(ctx, &pubkey, priv)) {
                // --- PROCESAR COMPRIMIDA (Moderno) ---
                size_t len_c = 33;
                secp256k1_ec_pubkey_serialize(ctx, pub_c, &len_c, &pubkey, SECP256K1_EC_COMPRESSED);
                SHA256(pub_c, 33, sha); RIPEMD160(sha, 32, h160_c);

                // --- PROCESAR NO COMPRIMIDA (Satoshi era) ---
                size_t len_u = 65;
                secp256k1_ec_pubkey_serialize(ctx, pub_u, &len_u, &pubkey, SECP256K1_EC_UNCOMPRESSED);
                SHA256(pub_u, 65, sha); RIPEMD160(sha, 32, h160_u);

                bool match_c = check_bloom(h160_c);
                bool match_u = check_bloom(h160_u);

                if (match_c || match_u) {
                    unsigned char* final_h160 = match_c ? h160_c : h160_u;
                    string h160_hex = to_hex(final_h160, 20);

                    if (binary_search(lista_objetivos_confirmados.begin(), lista_objetivos_confirmados.end(), h160_hex)) {
                        #pragma omp critical
                        {
/*-------------------------------------------------------------------------------------------------------------------------
# POSIBILIDADES DE ÉXITO
# Escenario,            key (HEX), formato,       addr1,              addr2
# Hallazgo Moderno,     abc123..., COMPRIMIDA,    1... (Legacy),      bc1q... (SegWit)
# Hallazgo Antiguo,     def456..., NO COMPRIMIDA, 1... (Satoshi era), bc1q... (Inusual)
#
# formato
#    Valor: Cómo se trató la clave pública para llegar a la dirección encontrada (COMPRIMIDA o NO COMPRIMIDA).
#        COMPRIMIDA: Indica que la clave pública se acortó a 33 bytes. Es lo estándar hoy en día.
#        NO COMPRIMIDA: Indica que la clave pública se usó en su tamaño original de 65 bytes. Es lo que se usaba en 2009-2010.
# hex key
#    Valor: Una cadena de 64 caracteres hexadecimales (0-9, a-f).
#           Es la Clave Privada en formato HEX. Es el "número secreto" puro del que sale todo lo demás.
# WIF key
#    Valor: El WIF (Wallet Import Format) es la key (hexadecimal) empaquetada de manera qeu detecte si es comprimida y un código de verificación (checksum), todo codificado en Base58.
#        - WIF con L o K: Si el formato es Comprimido, el WIF empezará por L o K. Las billeteras al leerlo ya saben que deben buscar en direcciones modernas.
#        - WIF con 5: Si el formato es No Comprimido, el WIF empezará por 5. Las billeteras sabrán que deben buscar en direcciones antiguas.
#    * Si hay éxito: Abre el archivo HALLAZGO.txt, copia el código WIF y pégalo en una cartera como Electrum (Pc), Sparrow Wallet (Pc) o BlueWallet (Móvil) (Menú Cartera -> Claves Privadas -> Importar).
# dirección objetivo
#    Valor: La dirección objetivo que el script usó para encontrar la coincidencia inicial. Si el objetivo era tipo 1...: Verás una dirección P2PKH (Legacy).
# dirección 'hermana'
#    Valor: "dirección hermana" generada a partir de la misma clave privada, por si acaso el dinero estuviera en el formato moderno. Una dirección Bech32 que empieza por bc1q.
-------------------------------------------------------------------------------------------------------------------------*/
                            found = true;
                            string format_found = match_c ? "COMPRIMIDA" : "NO COMPRIMIDA";
                            string addr_legacy = to_legacy_addr(final_h160);
                            string addr_segwit = to_bech32_addr(final_h160); 
                            string wif_c = to_wif(priv, true);
                            string wif_u = to_wif(priv, false);

                            cout << "\n\n" << string(60, '=') << endl;
                            cout << "   ¡¡¡ OBJETIVO REAL ENCONTRADO [" << format_found << "] !!!" << endl;
                            cout << "   DIRECCION 1... (Legacy): " << addr_legacy << endl;
                            cout << "   DIRECCION bc1q (SegWit): " << addr_segwit << endl;
                            cout << "   HASH160 (Identificador): " << h160_hex << endl;
                            cout << "   WIF COMPRIMIDO (K/L):    " << wif_c << endl;
                            cout << "   WIF UNCOMPR. (5):        " << wif_u << endl;
                            cout << string(60, '=') << endl;
                            
                            ofstream out("HALLAZGO.txt", ios::app);
                            out << "=========================================================" << endl;
                            out << "FECHA: " << get_current_time() << " | FORMATO: " << format_found << endl;
                            out << "DIRECCION HALLADA: " << addr_legacy << " / " << addr_segwit << endl;
                            out << "HASH160: " << h160_hex << endl;
                            out << "CLAVE PRIVADA (HEX): " << to_hex(priv, 32) << endl;
                            out << "WIF COMPRIMIDO: " << wif_c << endl;
                            out << "WIF NO COMPRIMIDO: " << wif_u << endl;
                            out << "=========================================================" << endl;
                            out.close();
                        }
                    } else {
                        #pragma omp atomic 
                            falsos_positivos++;
                    }
                }
            }
            #pragma omp atomic 
            total_keys++;

            if (total_keys % 1000000 == 0 && omp_get_thread_num() == 0) {
                auto now = chrono::high_resolution_clock::now();
                double elap = chrono::duration<double>(now - start_time).count();
                cout << "\r[Vel: " << fixed << setprecision(2) << (total_keys/elap)/1000 << " K/s] Tot: " << total_keys / 1000000 << "M | Falsos P.: " << falsos_positivos << " | " << to_hex(priv, 4) << "..." << flush;
            }
        }
        secp256k1_context_destroy(ctx);
    }
    cout << "\n\n Presione ENTER para salir..."; cin.get();
    return 0;
}