# Bitcoin Key Hunter
[🇺🇸 English] | [🇪🇸 Español](README.es.md)

High-speed CPU multi-threaded searcher for Bitcoin private keys. Supports Legacy, P2SH, SegWit, and Taproot (Bech32m) formats.

> [!TIP]
> The chances of success are not zero, but close. It is a pure brute-force process. It is not an investment; it is a lottery of cosmic dimensions where the prize only appears by an unpredictable statistical fluke. Success does not necessarily depend on time, but on an instantaneous stroke of luck.

## Project Content

The project consists of 2 main modules:

1.  **Bitcoin Key Hunter (bkh)**: Massive search engine in **C++**. Uses OpenMP for parallelism and libsecp256k1 for elliptic curve cryptography.
2.  **blockchair_addresses_filter**: **Python** script to process Blockchair dumps and extract binary identities (Hash160 and X-Only Pubkeys).

### Repository Organization

| Folder | Content |
| :--- | :--- |
| `/` | CMake and this README. |
| `/win_dll/` | Generated binaries and libraries (.dll) for Windows. Required to run, included for easy access. |
| `/src/` | C++ source code (bkh.cpp). |
| `/blockchair_addresses_filter/` | Python script v3.0 and target files. |

---

## RECOMMENDED COMPILATION (Via CMake)

### For WINDOWS (MSYS2)

1. **Environment Preparation**:
   - Install **MSYS2** (https://www.msys2.org/).
   - Open the **"MSYS2 MinGW 64-bit"** terminal.
   - Run:
      ```bash
      pacman -S --needed git mingw-w64-x86_64-gcc mingw-w64-x86_64-cmake mingw-w64-x86_64-openssl mingw-w64-x86_64-libgomp mingw-w64-x86_64-autotools
      ```

2. **libsecp256k1 Installation**:
   ```bash
   git clone https://github.com/bitcoin-core/secp256k1
   cd secp256k1
   ./autogen.sh
   ./configure --enable-module-recovery --enable-experimental --enable-module-schnorrsig --enable-module-extrakeys
   make -j$(nproc)
   make install
   ```

3. **Hunter Compilation**:
   Navigate to the project root and run:
   ```bash
   # Configure and generate build files
   cmake -B build_win -G "MinGW Makefiles"
   
   # Compile the project
   cmake --build build_win
   ```
   *Note: The executable and the lang.ini file will be generated in "build/bin/"*

---

### For LINUX (Ubuntu/Debian)

1. **Dependencies**:
   ```bash
   sudo apt update && sudo apt install build-essential cmake libssl-dev git automake libtool -y
   ```

2. **libsecp256k1 Installation**:
   (Follow the same steps as in Windows, but finish with privileges):
   ```bash
   sudo make install
   sudo ldconfig
   ```

3. **Compilation**:
   ```bash
   cmake -B build_linux
   cmake --build build_linux
   ```

---

## EXECUTION AND LIBRARIES (Windows)

### File Structure
To run the program, the destination folder must contain:
- bkh.exe (on Windows) or bkh (on Linux).
- lang.ini (Automatically copied to the bin folder by CMake).

### Required Libraries (DLLs)
To run the .exe outside of MSYS2 (always recommended via console), ensure these DLLs are in the same folder as the executable:
- libsecp256k1.dll, libcrypto-3-x64.dll, libgomp-1.dll, libwinpthread-1.dll, libstdc++-6.dll, libgcc_s_seh-1.dll.

---

## EXECUTION

The program is controlled via console:

- **Full CPU usage**: `./bkh` (or `bkh.exe`)
- **Free up cores**: `./bkh -n2` (leaves 2 threads free).
- **Custom file**: `` ./bkh -f"path/list.txt"`

---

## OBTAINING TARGETS

The Hunter does not search for random addresses; it searches for **Binary Identities**:
1.  **Legacy/P2SH/SegWit**: 40-character hex IDs (Hash160).
2.  **Taproot (bc1p)**: 64-character hex IDs (X-Only Pubkey).

**Process**:
1.  Download the .tsv.gz dump from **Blockchair**.
2.  Run `blockchair_addresses_filter.py` to generate `objetivos_identidad.txt`.
3.  Start the Hunter and select the address types to search for.

---

**Disclaimer**: This software is for educational and auditing purposes. The author is not responsible for its use.

---

# SUPPORT THE PROJECT

If this project has been useful to you and you want to support the development of **Bitcoin Key Hunter**, you can send a tip to the following addresses:

| Currency / Network | Address |
| :--- | :--- |
| **Bitcoin (BTC)** | `bc1qthc87aykdj2tjrxlqe7k9emz3txphrprm0scu6` |
| **Bitcoin (BTC) Lightning** | `ffg@cake.cash` |
| **Monero (XMR)** | `4A8mAK9SrCA3L2e7XuLxk8iWWfvNhqFVbTVz3q8zb1K6RisgTJtExWM3QPCzzheWz4GhcnMvCUKKfFoY869jPWBw1JxJnmj` |
| **ERC20/BEP20 (Ethereum / Polygon / BNB...)** | `0xb3F62CEB2706c9E7a27c2Aec6E3e1bA422f8097e` |
| **TRON (TRX) / USDT (TRC20)** | `TTmzFL2NZsD3yjHKCZg5Y7oBT8QXsJ8E6h` |
| **SOLANA (SOL)** | `2UGeSWJV6DaQEYS7fDtHW61EWQSoRd85JTYxHPrBjWaC` |
| **DOGECOIN (DOGE)** | `D85kgN3Q2CaxcjdANaYmeywcTsgBAGrfPJ` |
| **CARDANO (ADA)** | `addr1qxjl2vked39stz8jrgh5fj30gr8p5vh35nrl8g7asr0knw7s7j9gttx0xkq3zpjn220fql9yvzxw0agk0afnvg6fj75q7sqt95` |
| **SUI** | `0x55f06f745b058e0ab48300ff12bd08ac700fc2b23409fa942d81c5bce18864b7` |
| **BITCOIN CASH (BCH)** | `qqv9vptfw6vmcr75a35at33cy554uh52eun5t8pphu` |
| **ZCASH (ZEC)** | `u1wcufckcn8gfts4axlj9y5tjwp8ndy6l3mnfqapg4sszkdrj7adsjf0j8avzaestrkht2myrhn2l9m6q8nq5tqww884xavxm809rjk66lmtlrsfzygt78q7jp9024hjczjda5wvxsy7gc8l4fsnd078ac7pzrmrp9nqq8njejuu2h4h62` |

> [!TIP]
> For donations on the Ethereum network, you can send **ETH**, **POL (Polygon)**, **BNB**, or any **ERC20/BEP20** token to the same `0x` address. Thank you for your support!