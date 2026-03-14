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
- Comando de compilación desde la raíz del proyecto: g++ -O3 -march=native -fopenmp bkh.cpp -o bkh.exe -lsecp256k1 -lcrypto
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
