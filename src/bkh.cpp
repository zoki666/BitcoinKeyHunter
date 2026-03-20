/*
 * Bitcoin Key Hunter
 * Copyright (C) 2026 Francisco Fuertes Garcia
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
#      Buscador Multihilo Claves Bitcoin
# VERSION: 6.5
#
# REQUISITO EXTERNO:
# El archivo 'objetivos_identidad.txt' generado por el script de Python v2.4.
# - IDs de 40 hex (20 bytes): Legacy, P2SH y SegWit P2WPKH (bc1q corto).
# - IDs de 64 hex (32 bytes): EXCLUSIVAMENTE Taproot (bc1p).
#
# DIRECCIONES OBJETIVO VALIDAS:
# - Legacy (1...), P2SH (3...)
# - SegWit (bc1q...): Soporta tanto 42 como 62 caracteres.
# - Taproot (bc1p...): Soporta todas las variantes de Bech32m.
---------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------
# POSIBILIDADES DE EXITO (Busqueda Cuadruple de 160 bits)
# Escenario           | Key (HEX) | Formato        | Direccion 1 (Legacy) | Direccion 2 (P2SH) | Direccion 3 (SegWit) | Direccion 4 (Taproot)
# -------------------|-----------|----------------|----------------------|--------------------|----------------------|----------------------
# Hallazgo Moderno   | abc123... | COMPRIMIDA     | 1... (Estandar)      | 3... (Multisig/SW) | bc1q... (Native SW)  | bc1p... (Taproot)
# Hallazgo Antiguo   | def456... | NO COMPRIMIDA  | 1... (Satoshi era)   | 3... (Inusual)     | bc1q... (No estandar)| bc1p... (No estandar)
#
# FORMATO DE CLAVE PUBLICA:
#    Determina como se procesa la clave antes de aplicar el Hash160.
#    - COMPRIMIDA: Clave de 33 bytes. Estandar desde 2012. Genera WIFs que empiezan por 'L' o 'K'.
#    - NO COMPRIMIDA: Clave original de 65 bytes (usada en 2009-2011). Genera WIFs que empiezan por '5'.
#
# CLAVE PRIVADA (HEX):
#    El numero secreto de 256 bits (64 caracteres hex). Es la "madre" de todas las direcciones generadas.
#
# CLAVE WIF (Wallet Import Format):
#    Es la clave privada lista para importar en carteras (Electrum, Sparrow, BlueWallet).
#    - WIF L/K: Indica a la cartera que debe buscar fondos en direcciones modernas (Comprimidas).
#    - WIF 5:    Indica a la cartera que debe buscar en direcciones antiguas (No Comprimidas).
#
# EL CUADRUPLE "TARGET":
#    Tu programa ha encontrado una coincidencia con un objetivo. La misma clave privada
#    "abre" cuatro tipos de candados simultaneamente:
#    1. DIRECCION 1 (Legacy):  Formato original P2PKH.
#    2. DIRECCION 2 (P2SH):    Formato para Multifirmas o SegWit envuelto. 
#    3. DIRECCION 3 (bc1q):    Formato SegWit Nativo (Bech32).
#    4. DIRECCION 4 (bc1p):    Formato Taproot (Bech32m), basado en x-only pubkeys.
#
# PROCEDIMIENTO EN CASO DE EXITO:
#    1. Abre HALLAZGO.txt y localiza la Clave WIF o la Clave Privada HEX.
#    2. Importa el WIF en una cartera compatible (como Sparrow o Electrum).
#    3. Si el WIF es L/K, revisa los 4 tipos de direcciones; si es 5, revisa principalmente la Legacy.
-------------------------------------------------------------------------------------------------------------------------*/

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>
#include <chrono>
#include <omp.h>
#include <map>

#ifdef _WIN32
    #include <secp256k1/secp256k1.h>
    #include <secp256k1/secp256k1_extrakeys.h>
    #include <windows.h>
#else
    #include <secp256k1.h>
    #include <secp256k1_extrakeys.h>
#endif

#include <openssl/sha.h>
#include <openssl/ripemd.h>
#include <cstring>
#include <algorithm>
#include <random>
#include <csignal>
#include <limits>
#include <sstream>

using namespace std;

// --- TRADUCCIONES ---
map<string, string> T;

void load_languages() {
    string lang = "en"; 
#ifdef _WIN32
    LANGID langId = GetUserDefaultUILanguage();
    if (PRIMARYLANGID(langId) == LANG_SPANISH) lang = "es";
#endif

    // Buscamos lang.ini en la misma carpeta que el ejecutable
    ifstream file("lang.ini");
    string line;
    bool found_section = false;

    if (file.is_open()) {
        while (getline(file, line)) {
            // Limpieza de espacios y saltos de linea Windows (\r)
            line.erase(0, line.find_first_not_of(" \t\r\n"));
            line.erase(line.find_last_not_of(" \t\r\n") + 1);
            
            if (line.empty() || line[0] == '#') continue;
            
            if (line == "[" + lang + "]") { 
                found_section = true; 
                continue; 
            }
            if (line[0] == '[' && line.back() == ']') { 
                found_section = false; 
                continue; 
            }

            if (found_section) {
                size_t sep = line.find('=');
                if (sep != string::npos) {
                    string key = line.substr(0, sep);
                    string value = line.substr(sep + 1);
                    // Limpieza adicional de espacios alrededor del '='
                    key.erase(key.find_last_not_of(" \t") + 1);
                    T[key] = value;
                }
            }
        }
        file.close();
    }

    // Fallback de seguridad si no existe el archivo o la seccion
    if (T.empty()) {
        T["title"] = "BITCOIN KEY HUNTER v6.4";
        T["ask_file"] = "[?] File path";
        T["ask_threads"] = "[?] Threads to FREE (0 for all): ";
        T["ask_types"] = "Mark with + to include, or - to exclude.";
        T["search_legacy"] = "  [?] Search Legacy (1...) [+/-]: ";
        T["search_p2sh"] = "  [?] Search P2SH (3...) [+/-]: ";
        T["search_segwit"] = "  [?] Search SegWit (bc1q) [+/-]: ";
        T["search_taproot"] = "  [?] Search Taproot (bc1p) [+/-]: ";
        T["loading"] = "[i] Loading targets...";
        T["active_hilos"] = "[i] Active threads: ";
        T["stop_msg"] = "\n[!] Stopping search...";
        T["err_open"] = "[!] ERROR: Could not open: ";
        T["found_h160"] = "!!! H160 TARGET FOUND !!!";
        T["found_taproot"] = "!!! TAPROOT TARGET FOUND !!!";
    }
}

// --- VARIABLES GLOBALES ---
bool found = false;
bool keep_running = true;
bool buscar_legacy = false, buscar_p2sh = false, buscar_segwit = false, buscar_taproot = false;
unsigned long long total_keys = 0;
unsigned long long falsos_positivos = 0;

const uint64_t BLOOM_SIZE = 2147483648ULL; 
vector<bool> bloom_filter(BLOOM_SIZE, false);
vector<string> lista_h160;   
vector<string> lista_pub32;  

void signal_handler(int signum) {
    keep_running = false;
    cout << T["stop_msg"] << endl;
}

// --- UTILIDADES ---
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

void hex_to_bytes(string hex, unsigned char* bytes, int limit) {
    for (int i = 0; i < limit; i++) {
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
            digits[j] = carry % 58; carry /= 58;
        }
        while (carry) { digits[digits_len++] = carry % 58; carry /= 58; }
    }
    string res = "";
    for (size_t i = 0; i < len && data[i] == 0; i++) res += '1';
    for (size_t i = 0; i < digits_len; i++) res += ALPHABET[digits[digits_len - 1 - i]];
    return res;
}

string to_taproot_addr(const unsigned char* pub32) {
    static const char* charset = "qpzry9x8gf2tvdw0s3jn54khce6mua7l";
    vector<int> values = {2, 3, 10, 1, 1}; 
    uint32_t acc = 0, bits = 0;
    for (int i = 0; i < 32; ++i) {
        acc = (acc << 8) | pub32[i]; bits += 8;
        while (bits >= 5) { bits -= 5; values.push_back((acc >> bits) & 31); }
    }
    if (bits > 0) values.push_back((acc << (5 - bits)) & 31);
    vector<int> chk_v = values; 
    for(int i=0; i<6; i++) chk_v.push_back(0);
    uint32_t chk = 1;
    static uint32_t generator[] = {0x3b6a57b2, 0x26508e6d, 0x1ea119fa, 0x3d4233dd, 0x2a1462b3};
    for (int v : chk_v) {
        uint32_t top = chk >> 25;
        chk = ((chk & 0x1ffffff) << 5) ^ v;
        for (int i = 0; i < 5; i++) chk ^= ((top >> i) & 1) ? generator[i] : 0;
    }
    uint32_t mod = chk ^ 0x2bc830a3;
    string res = "bc1p";
    for (size_t i = 5; i < values.size(); ++i) res += charset[values[i]];
    for (int i = 0; i < 6; ++i) res += charset[(mod >> (5 * (5 - i))) & 31];
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

// --- MAIN ---
int main(int argc, char* argv[]) {
    load_languages();
    signal(SIGINT, signal_handler);
    
    int n_restar = 0;
    string file_path = "../../blockchair_addresses_filter/objetivos_identidad.txt"; 

    cout << "=================================================" << endl;
    cout << " " << T["title"] << endl;
    cout << "=================================================" << endl;

    string input;
    cout << T["ask_file"] << " [" << file_path << "]: ";
    getline(cin, input);
    if (!input.empty()) file_path = input;

    cout << T["ask_threads"];
    getline(cin, input);
    if (!input.empty()) n_restar = stoi(input);

    cout << T["ask_types"] << endl;
    cout << T["search_legacy"]; getline(cin, input); buscar_legacy = (input == "+");
    cout << T["search_p2sh"]; getline(cin, input); buscar_p2sh = (input == "+");
    cout << T["search_segwit"]; getline(cin, input); buscar_segwit = (input == "+");
    cout << T["search_taproot"]; getline(cin, input); buscar_taproot = (input == "+");

    ifstream file(file_path);
    if (!file.is_open()) { cout << T["err_open"] << file_path << endl; return 1; }

    cout << T["loading"] << endl;
    string line;
    while (getline(file, line)) {
        line.erase(remove_if(line.begin(), line.end(), ::isspace), line.end());
        if (line.empty()) continue;
        if (line.length() == 40) {
            unsigned char b[20]; hex_to_bytes(line, b, 20);
            add_to_bloom(b); lista_h160.push_back(line);
        } else if (line.length() == 64) {
            unsigned char b32[32]; hex_to_bytes(line, b32, 32);
            add_to_bloom(b32); lista_pub32.push_back(line);
        }
    }
    file.close();
    sort(lista_h160.begin(), lista_h160.end());
    sort(lista_pub32.begin(), lista_pub32.end());
    
    int hilos_uso = max(1, omp_get_num_procs() - n_restar);
    omp_set_num_threads(hilos_uso);

    cout << "[i] H160: " << lista_h160.size() << " | P2TR: " << lista_pub32.size() << endl;
    cout << T["active_hilos"] << hilos_uso << endl;
    cout << "=================================================\n" << endl;

    auto start_time = chrono::high_resolution_clock::now();

    #pragma omp parallel
    {
        secp256k1_context* ctx = secp256k1_context_create(SECP256K1_CONTEXT_SIGN);
        unsigned char priv[32], h160_c[20], pub_c[33], sha[32], pub32[32];
        mt19937_64 engine(omp_get_thread_num() + (unsigned int)time(0));
        uniform_int_distribution<uint64_t> dist(0, 0xFFFFFFFFFFFFFFFFULL);

        while (keep_running && !found) {
            for(int i=0; i<4; i++) { uint64_t r = dist(engine); memcpy(priv + (i*8), &r, 8); }

            secp256k1_pubkey pubkey;
            if (secp256k1_ec_pubkey_create(ctx, &pubkey, priv)) {
                if (buscar_legacy || buscar_p2sh || buscar_segwit) {
                    size_t len_c = 33;
                    secp256k1_ec_pubkey_serialize(ctx, pub_c, &len_c, &pubkey, SECP256K1_EC_COMPRESSED);
                    SHA256(pub_c, 33, sha); RIPEMD160(sha, 32, h160_c);
                    if (check_bloom(h160_c)) {
                        string h160_hex = to_hex(h160_c, 20);
                        if (binary_search(lista_h160.begin(), lista_h160.end(), h160_hex)) {
                            #pragma omp critical
                            {
                                found = true;
                                cout << T["found_h160"] << endl;
                                cout << "WIF: " << to_wif(priv, true) << " | HEX: " << to_hex(priv, 32) << endl;
                                ofstream out("HALLAZGO.txt", ios::app);
                                out << "FECHA: " << get_current_time() << " | PRIV: " << to_hex(priv, 32) << " | WIF: " << to_wif(priv, true) << endl;
                                out.close();
                            }
                        } else {
                            #pragma omp atomic
                            falsos_positivos++;
                        }
                    }
                }
                if (!found && buscar_taproot) {
                    secp256k1_xonly_pubkey xonly_pub;
                    secp256k1_xonly_pubkey_from_pubkey(ctx, &xonly_pub, NULL, &pubkey);
                    secp256k1_xonly_pubkey_serialize(ctx, pub32, &xonly_pub);
                    if (check_bloom(pub32)) {
                        string pub32_hex = to_hex(pub32, 32);
                        if (binary_search(lista_pub32.begin(), lista_pub32.end(), pub32_hex)) {
                            #pragma omp critical
                            {
                                found = true;
                                string addr_taproot = to_taproot_addr(pub32);
                                cout << T["found_taproot"] << endl;
                                cout << "ADDR: " << addr_taproot << "\nWIF: " << to_wif(priv, true) << endl;
                                ofstream out("HALLAZGO.txt", ios::app);
                                out << "FECHA: " << get_current_time() << " | TAPROOT: " << addr_taproot << " | WIF: " << to_wif(priv, true) << endl;
                                out.close();
                            }
                        } else {
                            #pragma omp atomic
                            falsos_positivos++;
                        }
                    }
                }
            }
            #pragma omp atomic 
            total_keys++;

            if (total_keys % 100000 == 0 && omp_get_thread_num() == 0) {
                auto now = chrono::high_resolution_clock::now();
                double elap = chrono::duration<double>(now - start_time).count();
                double v_khs = (total_keys / elap) / 1000.0;
                cout << "\r[Vel: " << fixed << setprecision(2) << v_khs << " K/s] "
                     << "Tot: " << (total_keys / 1000000.0) << "M | "
                     << "FP: " << falsos_positivos << " | "
                     << to_hex(priv, 4) << "..." << flush;
            }
        }
        secp256k1_context_destroy(ctx);
    }
    return 0;
}