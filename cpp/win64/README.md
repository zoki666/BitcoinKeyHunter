# Bitcoin Key Hunter

Versión para Windows.

## Explicación de los elementos del proyecto:

- bkh.cpp: Código fuente

- Librerías de Funciones Básicas y Sistema:
    - libwinpthread-1.dll: Permite que el programa use hilos (threads). Es lo que permite que una aplicación realice varias tareas al mismo tiempo en Windows siguiendo el estándar POSIX.
    - libgcc_s_seh-1.dll: Es una librería base del compilador GCC. Se encarga de la gestión de excepciones (errores) y de funciones matemáticas básicas de bajo nivel.
    - libstdc++-6.dll: Contiene todas las funciones estándar del lenguaje C++ (manejo de textos, listas de datos, entrada y salida de archivos, etc.). Sin ella, casi ningún programa moderno de C++ funciona.

- Librerías de Criptografía y Seguridad:
    - libcrypto-3-x64.dll: Es parte de OpenSSL. Proporciona herramientas de cifrado, hashing y protocolos de seguridad para proteger datos.
    - libsecp256k1-6.dll: Esta es muy específica. Se utiliza para criptografía de curva elíptica. Es extremadamente común en aplicaciones de Bitcoin y criptomonedas para gestionar claves públicas y privadas.

- Librería de Rendimiento:
    - libgomp-1.dll: Es la librería de OpenMP. Sirve para el "procesamiento paralelo", permitiendo que el programa use todos los núcleos de tu procesador para ir más rápido en cálculos pesados.

- Fichero de texto con direcciones objetivos de los que pretende obtener acceso (ver más abajo): *objetivos_hash160.txt* --> lista de direcciones reales hash160 (40 caracteres)


## COMPILACIÓN (según mi recomendación):
- Instalar MSYS2 y utilizar la terminal MINGW64.
- Desde la terminal MINGW64, instalar las dependencias necesarias: pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-openssl --needed
    Nota sobre libsecp256k1: Si el paquete no se encuentra en el repositorio oficial (he tenido problemas para encontrarlo en ocasiones, búscalo manualmente enotros sitios de confianza), asegúrate de tener los archivos de cabecera (.h) en /mingw64/include/secp256k1 y las librerías estáticas (.a) en /mingw64/lib.
- Para que la compilación sea exitosa, verifica que tu entorno tenga:
    Librerías: libsecp256k1.a y libcrypto.a (dentro de /mingw64/lib/).
    Cabeceras: Carpeta /mingw64/include/openssl/ y /mingw64/include/secp256k1/.
- Comando de compilación desde la raíz del proyecto:
  ```bash
  g++ -O3 -march=native -fopenmp bkh.cpp -o bkh.exe -lsecp256k1 -lcrypto
  ```
Explicación de los parámetros:
    -O3: Activa las optimizaciones de velocidad de alto nivel.
    -march=native: Vital. Optimiza el ejecutable específicamente para el modelo de CPU (i5, i7, etc.).
    -fopenmp: Habilita el uso de múltiples núcleos (multi-threading).
    -lsecp256k1 -lcrypto: Enlaza las librerías matemáticas y criptográficas necesarias.

> [!NOTE]
> Solución de Problemas Comunes
> - Warning "Deprecated RIPEMD160": Es normal en versiones de OpenSSL 3.0+. No afecta al funcionamiento ni a la velocidad.
> - Error "Fatal error: openssl/sha.h": Falta el paquete de desarrollo de OpenSSL. Reinstala con pacman -S mingw-w64-x86_64-openssl.
> - Error "Cannot find -lsecp256k1": Verifica que el archivo libsecp256k1.a esté en la carpeta lib de tu instalación de MSYS2.

## EJECUCIÓN:
- Tras compilar se generará el ejecutable: bkh.exe

Ejecutar desde un terminal:
- Usar todos los núcleos: bkh.exe
- Dejar 2 núcleos libres: bkh.exe -n2
    * Si el parametro de entrada > cantidad de núcleos, se usará sólo 1
- Dejar 2 núcleos libres y usar un fichero de objetivos específico: bkh.exe -n2 -f"C:\Mis Documentos\lista bitcoin.txt"
    * Si no se indica fichero se usa objetivos_hash160.txt de la misma ruta que el ejecutable

### ¿Cómo obtener el fichero con direcciones objetivo?
Para que funcione, el archivo objetivos_hash160.txt debe contener los hashes en hexadecimal (40 caracteres), no las direcciones 1... directamente. 
El propio código advierte que debes convertir 1A1zP... a su equivalente hash160 antes de empezar.
Usando el script en python **blockchair_addresses_filter** que a partir de un dump bajade de blockchair genera el fichero!

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
