# ---------------------------------------------------------------------------------------
# blockchair_addresses_filter.py
# Copyright (C) 2026 Francisco Fuertes Garcia
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# ---------------------------------------------------------------------------------------
# SCRIPT: Blockchair Addresses Filter (blockchair_addresses_filter.py)
#         Generador de objetivos.txt y objetivos_identidad.txt (H160 + Taproot)
# VERSION: 3.0 (Soporte Universal + Limpieza Agresiva de Datos)
# ---------------------------------------------------------------------------------------
# DIRECCIONES OBJETIVO VALIDAS (SOLO SINGLE-SIG / ACCESO DIRECTO):
#
# 1. LEGACY (1...) y P2SH (3...): 
#    - Extrae el HashID (Hash160 o ScriptHash de 20 bytes / 40 chars hex).
#
# 2. SEGWIT NATIVO (bc1q...): 
#    - SOLO P2WPKH (Addr 42 chars): Extrae el Witness Program (20 bytes / 40 chars hex).
#    - P2WSH (Addr > 42 chars): IGNORADAS. Suelen ser Multisig o Scripts complejos.
#
# 3. TAPROOT (bc1p...): 
#    - Soporta variaciones de longitud (62-64 chars addr).
#    - Extrae la X-Only Public Key (32 bytes / 64 chars hex).
# ---------------------------------------------------------------------------------------
# NOTA: Este script garantiza que cada ID generado corresponde a una clave privada 
# unica. Si BitcoinKeyHunter (C++) encuentra el "Match", el control de los fondos 
# es inmediato mediante la firma Schnorr (Taproot) o ECDSA (Legacy/SegWit).
# ---------------------------------------------------------------------------------------

import sys
import os
import gzip
import time
import importlib.util
import subprocess

# Gestion de dependencias (Solo base58 para Legacy/P2SH)
if importlib.util.find_spec("base58") is None:
    print("[+] Instalando dependencia necesaria: base58...")
    subprocess.check_call([sys.executable, "-m", "pip", "install", "base58"])

import base58

# --- CONFIGURACION ---
ARCHIVO_ENTRADA = "blockchair_bitcoin_addresses_latest.tsv.gz"
SALIDA_ID = "objetivos_identidad.txt"
SALIDA_ADDR = "objetivos.txt"
MIN_SATOSHIS = 10000

def addr_to_identity(address):
    try:
        address = address.strip()
        # Alfabeto Bech32 oficial (BIP173 / BIP341)
        CHARSET = "qpzry9x8gf2tvdw0s3jn54khce6mua7l"

        # 1. LEGACY / P2SH (1... o 3...)
        if address.startswith(('1', '3')):
            return base58.b58decode_check(address)[1:].hex()

        # 2. SEGWIT / TAPROOT (bc1...)
        if address.startswith('bc1'):
            pos = address.rfind('1')
            if pos < 2: return None
            
            data_str = address[pos+1:-6] # Excluir prefijo y checksum
            try:
                # Mapeo de bits a valores de 5-bits
                values = [CHARSET.find(c) for c in data_str]
                if -1 in values: return None
                
                # Conversion de base32 (5-bits) a hex (8-bits)
                acc = 0
                bits = 0
                ret = []
                for value in values[1:]: # Omitir witness version
                    acc = (acc << 5) | value
                    bits += 5
                    while bits >= 8:
                        bits -= 8
                        ret.append((acc >> bits) & 0xff)
                
                res = bytes(ret).hex()
                
                # Validacion de longitud por tipo de direccion
                # P2WPKH (bc1q corto) -> 20 bytes
                if address.startswith('bc1q') and len(address) == 42:
                    if len(res) == 40: return res
                
                # P2TR (bc1p) -> 32 bytes (X-Only Public Key)
                if address.startswith('bc1p'):
                    if len(res) == 64: return res
                    
            except: return None
    except: pass
    return None

# --- INICIO DEL PROCESO ---
print("\n" + "="*60)
print(" BITCOIN ADDRESS FILTRATOR v3.0 - PRODUCTION READY")
print("="*60)
print(f"[+] Archivo: {ARCHIVO_ENTRADA}")
print(f"[+] Minimo:  {MIN_SATOSHIS} satoshis")
print("="*60 + "\n")

count_total = 0
count_h160 = 0
count_ext = 0
start_time = time.time()

if not os.path.exists(ARCHIVO_ENTRADA):
    print(f"[!] ERROR: No se encuentra {ARCHIVO_ENTRADA}")
    sys.exit(1)

tamanio_total = os.path.getsize(ARCHIVO_ENTRADA)

try:
    with open(ARCHIVO_ENTRADA, 'rb') as f_raw:
        with gzip.open(f_raw, 'rt', encoding='utf-8') as f_in, \
             open(SALIDA_ID, 'w') as f_id, \
             open(SALIDA_ADDR, 'w') as f_ad:
            
            f_in.readline() # Saltar cabecera TSV
            
            for line in f_in:
                count_total += 1
                line = line.strip()
                if not line: continue
                
                parts = line.split('\t')
                if len(parts) < 2: parts = line.split()
                if len(parts) < 2: continue
                
                address = parts[0].strip()
                
                try:
                    # Limpieza agresiva de balance (solo digitos)
                    balance_str = ''.join(c for c in parts[1] if c.isdigit())
                    balance = int(balance_str)
                except: continue
                
                if balance >= MIN_SATOSHIS:
                    identidad = addr_to_identity(address)
                    
                    if identidad:
                        if len(identidad) == 64:
                            count_ext += 1
                            f_id.write(identidad + "\n")
                            f_ad.write(address + "\n")
                        elif len(identidad) == 40:
                            count_h160 += 1
                            f_id.write(identidad + "\n")
                            f_ad.write(address + "\n")
                
                # Actualizacion de progreso cada 500k lineas
                if count_total % 500000 == 0:
                    progreso = (f_raw.tell() / tamanio_total) * 100
                    sys.stdout.write(f"\r[+] {progreso:.1f}% | H160: {count_h160} | P2TR: {count_ext} | Leidas: {count_total}")
                    sys.stdout.flush()

except KeyboardInterrupt:
    print("\n[!] Proceso interrumpido por el usuario.")
except Exception as e:
    print(f"\n[!] ERROR CRITICO: {e}")

# --- ESTADISTICAS FINALES ---
duracion = time.time() - start_time
print("\n\n" + "="*60)
print(" ESTADISTICAS DE FILTRADO")
print("="*60)
print(f"[+] Tiempo total:    {duracion:.2f} segundos")
print(f"[+] Objetivos H160:  {count_h160:,}")
print(f"[+] Objetivos P2TR:  {count_ext:,}")
print(f"[+] Total exportado: {count_h160 + count_ext:,}")
print("-" * 60)
print(f"[*] Archivos generados: {SALIDA_ID}, {SALIDA_ADDR}")
print("="*60 + "\n")