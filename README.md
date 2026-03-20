# Bitcoin Key Hunter

Buscador multihilo de alta velocidad con CPU para claves privadas de Bitcoin. Soporta formatos Legacy, P2SH, SegWit y Taproot (Bech32m).

> [!TIP]
> Las posibilidades de exito no son cero, pero casi. Es un proceso de fuerza bruta pura. No es una inversion, es una loteria de dimensiones cosmicas donde el premio solo aparece por una carambola estadistica impredecible. El exito no depende necesariamente del tiempo, sino de un golpe de suerte instantaneo.

## Contenido del Proyecto

El proyecto consta de 2 modulos principales:

1.  **Bitcoin Key Hunter (bkh)**: Motor de busqueda masiva en **C++**. Utiliza OpenMP para paralelismo y libsecp256k1 para criptografia de curva eliptica.
2.  **blockchair_addresses_filter**: Script en **Python** para procesar dumps de Blockchair y extraer las identidades binarias (Hash160 y X-Only Pubkeys).

### Organizacion del Repositorio

| Carpeta | Contenido |
| :--- | :--- |
| `/` | CMake y este README. |
| `/bin/` | Binarios generados y librerias (.dll) para Windows. |
| `/src/` | Codigo fuente en C++ (bkh.cpp). |
| `/blockchair_addresses_filter/` | Script Python v3.0 y ficheros de objetivos. |

---

## COMPILACION RECOMENDADA (Via CMake)

### Para WINDOWS (MSYS2)

1. **Preparacion del Entorno**:
   - Instala **MSYS2** (https://www.msys2.org/).
   - Abre la terminal **"MSYS2 MinGW 64-bit"**.
   - Ejecuta:
     ```bash
     pacman -S --needed git mingw-w64-x86_64-gcc mingw-w64-x86_64-cmake mingw-w64-x86_64-openssl mingw-w64-x86_64-libgomp mingw-w64-x86_64-autotools
     ```

2. **Instalacion de libsecp256k1**:
   ```bash
   git clone https://github.com/bitcoin-core/secp256k1
   cd secp256k1
   ./autogen.sh
   ./configure --enable-module-recovery --enable-experimental --enable-module-schnorrsig --enable-module-extrakeys
   make -j$(nproc)
   make install
   ```

3. **Compilacion del Hunter**:
   Navega a la raiz del proyecto y ejecuta:
   ```bash
   # Configurar y generar archivos de construccion
   cmake -B build_win -G "MinGW Makefiles"
   
   # Compilar el proyecto
   cmake --build build_win
   ```
   *Nota: El ejecutable y el archivo lang.ini se generaran en "build/bin/"*

---

### Para LINUX (Ubuntu/Debian)

1. **Dependencias**:
   ```bash
   sudo apt update && sudo apt install build-essential cmake libssl-dev git automake libtool -y
   ```

2. **Instalacion de libsecp256k1**:
   (Sigue los mismos pasos que en Windows, pero finaliza con privilegios):
   ```bash
   sudo make install
   sudo ldconfig
   ```

3. **Compilacion**:
   ```bash
   cmake -B build_linux
   cmake --build build_linux
   ```

---

## EJECUCION Y LIBRERIAS (Windows)

### Estructura de archivos
Para ejecutar el programa, la carpeta de destino debe contener:
- bkh.exe (en Windows) o bkh (en Linux).
- lang.ini (Copiado automaticamente a la carpeta bin por CMake).

### Librerias Requeridas (DLLs)
Para ejecutar el .exe fuera de MSYS2 (recomendable siempre por consola), asegurese de que estas DLLs esten en la misma carpeta que el ejecutable:
- libsecp256k1.dll, libcrypto-3-x64.dll, libgomp-1.dll, libwinpthread-1.dll, libstdc++-6.dll, libgcc_s_seh-1.dll.

---

## EJECUCION

El programa se controla por consola:

- **Uso total de CPU**: `./bkh` (o `bkh.exe`)
- **Liberar nucleos**: `./bkh -n2` (deja 2 hilos libres).
- **Fichero personalizado**: `./bkh -f"ruta/lista.txt"`

---

## OBTENCION DE OBJETIVOS

El Hunter no busca direcciones al azar, busca **Identidades Binarias**:
1.  **Legacy/P2SH/SegWit**: IDs de 40 caracteres hex (Hash160).
2.  **Taproot (bc1p)**: IDs de 64 caracteres hex (X-Only Pubkey).

**Proceso**:
1.  Descarga el dump .tsv.gz de **Blockchair**.
2.  Ejecuta `blockchair_addresses_filter.py` para generar `objetivos_identidad.txt`.
3.  Inicia el Hunter y selecciona los tipos de direccion a buscar.

---

**Aviso**: Este software es para fines educativos y de auditoria. El autor no se hace responsable de su uso.

---

# APOYA EL PROYECTO

Si este proyecto te ha sido útil y quieres apoyar el desarrollo de **Bitcoin Key Hunter**, puedes enviar una propina a las siguientes direcciones:

| Moneda / Red | Dirección |
| :--- | :--- |
| **Bitcoin (BTC)** | `bc1qmjuxnynmwqy4sr8nsy7q9w2f0q3c450ry57s4v` |
| **Monero (XMR)** | `4A8mAK9SrCA3L2e7XuLxk8iWWfvNhqFVbTVz3q8zb1K6RisgTJtExWM3QPCzzheWz4GhcnMvCUKKfFoY869jPWBw1JxJnmj` |
| **ERC20/BEP20 (Ethereum / Polygon / BNB...)** | `0xb3F62CEB2706c9E7a27c2Aec6E3e1bA422f8097e` |
| **TRON (TRX) / USDT (TRC20)** | `TTmzFL2NZsD3yjHKCZg5Y7oBT8QXsJ8E6h` |
| **SOLANA (SOL)** | `2UGeSWJV6DaQEYS7fDtHW61EWQSoRd85JTYxHPrBjWaC` |
| **DOGECOIN (DOGE)** | `D85kgN3Q2CaxcjdANaYmeywcTsgBAGrfPJ` |
| **CARDANO (ADA)** | `addr1qxjl2vked39stz8jrgh5fj30gr8p5vh35nrl8g7asr0knw7s7j9gttx0xkq3zpjn220fql9yvzxw0agk0afnvg6fj75q7sqt95` |
| **SUI** | `0x55f06f745b058e0ab48300ff12bd08ac700fc2b23409fa942d81c5bce18864b7` |
| **BITCOIN CASH (BCH)** | `qqv9vptfw6vmcr75a35at33cy554uh52eun5t8pphu` |
| **ZCASH (ZEC)** | `t1JE2AQFGMWVRM3bwXLDPLKUAGpF4QUrsjj` |

> [!TIP]
> Para las donaciones en la red de Ethereum, puedes enviar tanto **ETH** como **POL (Polygon)**, **BNB** o cualquier token **ERC20/BEP20** a la misma dirección `0x`. ¡Gracias por tu apoyo!