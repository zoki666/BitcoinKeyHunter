# ---------------------------------------------------------
 # blockchair_addresses_filter.py
 # Copyright (C) 2026 Francisco Fuertes García
 #
 # This program is free software: you can redistribute it and/or modify
 # it under the terms of the GNU General Public License as published by
 # the Free Software Foundation, either version 3 of the License, or
 # (at your option) any later version.
 #
 # This program is distributed in the hope that it will be useful,
 # but WITHOUT ANY WARRANTY; without even the implied warranty of
 # MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 # GNU General Public License for more details.
 #
# ---------------------------------------------------------
# SCRIPT: Blockchair Addresses Filter (blockchair_addresses_filter.py)
#     Generador de objetivos.txt y objetivos_hash160.txt a partir de blockchair_bitcoin_addresses_latest.tsv.gz
# VERSIÓN: 1.0
#
# DEPENDENCIAS DE LIBRERÍAS:
# Hay dependencia de las siguientes librerías. Se instalan automáticamente al ejecutar el script.
#    - base58: Añade el prefijo 0x00 y el checksum para crear la dirección 1....
#    - bech32: Codifica el mismo hash de 20 bytes para crear la dirección bc1q....
# Una instalación de las librerías manualmente, sería como sigue:
#    - Unix: pip install base58 bech32
#    - Windows (si da error de permisos la anterior): pip install --user base58 bech32
#
# DIRECCIONES OBJETIVO VÁLIDAS:
# - Direcciones Legacy (empiezan por 1 y tienen entre 26 y 35 caracteres)
# - Direcciones SegWit (empiezan por bc1q y tienen exactamente 42 caracteres)
# - Requisito de saldo mínimo (MIN_SATOSHIS) por defecto 0.00001btc
#
# EJECUCIÓN:
# python blockchair_addresses_filter.py
# ---------------------------------------------------------

import sys
import subprocess
import os
import gzip
import time
import importlib.util

# --- AUTO-INSTALADOR DE LIBRERÍAS ---
def install_dependencies():
    required = ['base58', 'bech32']
    for lib in required:
        spec = importlib.util.find_spec(lib)
        if spec is None:
            print(f"[*] Instalando librería necesaria: {lib}...")
            try:
                subprocess.check_call([sys.executable, "-m", "pip", "install", lib])
                print(f"[+] {lib} instalada correctamente.")
            except Exception as e:
                print(f"[!] Error instalando {lib}: {e}")
                sys.exit(1)

install_dependencies()

import base58
import bech32

# --- CONFIGURACIÓN ---
ARCHIVO_ENTRADA = "blockchair_bitcoin_addresses_latest.tsv.gz"
SALIDA_TXT = "objetivos.txt"
SALIDA_HASH = "objetivos_hash160.txt"
MIN_SATOSHIS = 1000  # 0.00001 BTC (1000 Satoshis)

def addr_to_h160(addr):
    try:
        # Direcciones Legacy (empiezan por 1)
        if addr.startswith('1'):
            decoded = base58.b58decode_check(addr)
            return decoded[1:].hex()
        
        # Direcciones SegWit Native (empiezan por bc1q)
        elif addr.startswith('bc1q'):
            hrp, data, spec = bech32.bech32_decode(addr)
            if data is None: return None
            decoded = bech32.convertbits(data[1:], 5, 8, False)
            return bytes(decoded).hex()
    except:
        return None
    return None

# --- INICIO DEL PROCESO ---
if not os.path.exists(ARCHIVO_ENTRADA):
    print(f"\n[ERROR] No se encuentra el archivo: {ARCHIVO_ENTRADA}")
    print("Asegúrate de que el archivo .tsv.gz esté en esta misma carpeta.")
    sys.exit(1)

tamanio_total = os.path.getsize(ARCHIVO_ENTRADA)
ancho_barra = 30

print(f"\n" + "="*60)
print(f" BITCOIN ADDRESS FILTRATOR & HASH160 CONVERTER")
print("="*60)
print(f"[*] Origen:  {ARCHIVO_ENTRADA} ({tamanio_total / 1024 / 1024:.2f} MB)")
print(f"[*] Filtro:  >= {MIN_SATOSHIS/100000000} BTC")
print(f"[*] Salida:  {SALIDA_TXT} y {SALIDA_HASH}")
print("="*60 + "\n")

count_total = 0
count_validos = 0
count_saltados_saldo = 0
count_saltados_tipo = 0
start_time = time.time()

try:
    # Abrimos archivos. El modo 'w' los vacía si ya existen.
    with open(ARCHIVO_ENTRADA, 'rb') as f_raw:
        with gzip.open(f_raw, 'rt', encoding='utf-8') as f_in, \
             open(SALIDA_TXT, 'w') as f_txt, \
             open(SALIDA_HASH, 'w') as f_hash:
            
            # Saltamos la línea de encabezado de Blockchair
            f_in.readline()
            
            for line in f_in:
                count_total += 1
                parts = line.split('\t')
                
                if len(parts) < 2: 
                    continue
                
                address = parts[0]
                try:
                    balance = int(parts[1])
                except: 
                    continue
                
                # 1. Filtro de Saldo
                if balance >= MIN_SATOSHIS:
                    # 2. Conversión a Hash160 (Solo tipos 1... y bc1q...)
                    h160 = addr_to_h160(address)
                    
                    if h160:
                        # Escribimos en ambos ficheros para mantener sincronía total
                        f_txt.write(address + "\n")
                        f_hash.write(h160 + "\n")
                        count_validos += 1
                    else:
                        count_saltados_tipo += 1
                else:
                    count_saltados_saldo += 1
                
                # Actualización de la barra de progreso (cada 200k líneas para suavidad)
                if count_total % 200000 == 0:
                    pos_actual = f_raw.tell()
                    progreso = pos_actual / tamanio_total
                    bloques = int(ancho_barra * progreso)
                    barra = "█" * bloques + "-" * (ancho_barra - bloques)
                    
                    tiempo_transcurrido = time.time() - start_time
                    eta_seconds = int((tiempo_transcurrido / progreso) - tiempo_transcurrido) if progreso > 0 else 0
                    
                    # Formatear ETA a HH:MM:SS
                    eta_format = time.strftime("%H:%M:%S", time.gmtime(eta_seconds))
                    
                    # Velocidad en líneas por segundo
                    lps = count_total / tiempo_transcurrido
                    
                    sys.stdout.write(f"\r|{barra}| {progreso*100:>5.1f}% | Encontrados: {count_validos:>8} | {lps:>7.0f} L/s | ETA: {eta_format} ")
                    sys.stdout.flush()

except KeyboardInterrupt:
    print("\n\n[!] Proceso detenido por el usuario (CTRL+C).")

total_time = time.time() - start_time
print(f"\n\n" + "="*60)
print(f" RESUMEN FINAL")
print("="*60)
print(f"[+] Tiempo total:         {time.strftime('%H:%M:%S', time.gmtime(total_time))}")
print(f"[+] Líneas analizadas:    {count_total:,}")
print(f"    - Guardadas (Match):  {count_validos:,}")
print(f"    - Saldo < 0.0001 BTC: {count_saltados_saldo:,}")
print(f"    - Tipos descartados:  {count_saltados_tipo:,}")
print("-"*60)
print(f"[*] Los archivos están listos para usar en la carpeta bkh.")
print("="*60)