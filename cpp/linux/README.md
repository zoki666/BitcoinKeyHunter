# Bitcoin Key Hunter

Versión para linux.

## Explicación de los elementos del proyecto:

- bkh.cpp: Código fuente

- Fichero de texto con direcciones objetivos de los que pretende obtener acceso (ver abajo): objetivos_hash160.txt --> lista de direcciones reales hash160 (40 caracteres)

## LIBRERÍAS:

- Librerías de Funciones Básicas y Sistema:
    - libpthread.so (POSIX threads): Es la librería que gestiona los hilos de ejecución. Permite que el programa realice múltiples tareas en paralelo, algo fundamental para no desperdiciar la potencia de CPUs con muchos núcleos.
    - libstdc++.so.6 & libgcc_s.so.1: Son las librerías base del compilador GNU. Proporcionan todas las funciones estándar de C++ (manejo de datos, memoria y excepciones) y operaciones matemáticas de bajo nivel.
    - libc.so.6 (glibc): Es la librería fundamental de Linux. Actúa como el puente entre el programa y el Kernel (núcleo) del sistema operativo para tareas básicas como leer archivos o gestionar el teclado.
- Librerías de Criptografía y Seguridad:
    - libcrypto.so.3 (OpenSSL): Proporciona el motor de hashing. Se encarga de calcular los hashes SHA256 y RIPEMD160 necesarios para transformar llaves públicas en direcciones de Bitcoin.
    - libsecp256k1.so.6: Es la librería más crítica. Realiza los cálculos de curva elíptica de forma optimizada. Al compilarla manualmente en el sistema, aseguramos que aproveche las instrucciones específicas de tu procesador (como AVX2) para generar claves privadas a la máxima velocidad posible.
- Librería de Rendimiento y Paralelismo:
    - libgomp.so.1 (OpenMP): Es la pieza maestra para la aceleración. Permite que el código se reparta entre todos los núcleos disponibles de la CPU, logrando que el escaneo sea proporcional a la potencia de tu hardware.

## COMPILACIÓN:
- Instala las herramientas esenciales de compilación y las librerías de desarrollo de OpenSSL:
  ```bash
  sudo apt update && sudo apt upgrade -y
  sudo apt install build-essential libssl-dev git automake libtool -y
  ```
- Para garantizar la máxima compatibilidad y velocidad, compilar las librerías directamente en el sistema:
    - Ir a la carpeta personal y clonar
      ```bash
      cd ~
      git clone https://github.com/bitcoin-core/secp256k1
      cd secp256k1
      ```

    - Configurar y compilar
      ```bash
      ./autogen.sh
      ./configure --enable-module-recovery --enable-experimental --enable-module-schnorrsig
      make -j$(nproc)
      ```

    - Instalar en el sistema
      ```bash
      sudo make install
      sudo ldconfig
      ```
      
- Navega hasta la carpeta donde se encuentra tu archivo bkh.cpp (raíz del proyecto) y compila:
  ```bash
  g++ -O3 -march=native -fopenmp bkh.cpp -o bkh -lsecp256k1 -lcrypto
  ```

## EJECUCIÓN:
- Tras compilar se generará el binario: bkh (se recomienda ejecutarlo desde consola)

- Usar todos los núcleos: ./bkh
- Dejar 2 núcleos libres: ./bkh -n2
    * Si el parametro de entrada > cantidad de núcleos, se usará sólo 1
- Dejar 2 núcleos libres y usar un fichero de objetivos específico: ./bkh -n2 -f"/home/user/lista bitcoin.txt"
    * Si no se indica fichero se usa objetivos_hash160.txt de la misma ruta que el ejecutable

### ¿Cómo obtener el fichero con direcciones objetivo?
Para que funcione, el archivo objetivos_hash160.txt debe contener los hashes en hexadecimal (40 caracteres), no las direcciones 1... directamente. 
El propio código advierte que debes convertir 1A1zP... a su equivalente hash160 antes de empezar.
¡Usando el script en python **blockchair_addresses_filter** que a partir de un dump bajade de blockchair genera el fichero!

# Apoya el proyecto

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

# Licencia (ver LICENSE para más detalle)

Este programa es software libre: puedes redistribuirlo y/o modificarlo bajo los términos de la Licencia Pública General GNU (GPLv3) según lo publicado por la Free Software Foundation.

Este programa se distribuye con la esperanza de que sea útil, pero SIN NINGUNA GARANTÍA; incluso sin la garantía implícita de COMERCIABILIDAD o IDONEIDAD PARA UN PROPÓSITO PARTICULAR. 

Al ser una licencia GPLv3:

1\. Debes citarme como autor original.

2\. Cualquier trabajo derivado de este código debe ser también abierto y bajo esta misma licencia.

3\. El autor no se hace responsable de los daños o el uso que se le dé a esta herramienta.
