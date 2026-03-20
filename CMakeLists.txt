cmake_minimum_required(VERSION 3.10)
project(BitcoinKeyHunter LANGUAGES CXX)

# 1. Configuracion del Compilador
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Opciones de compilacion universales (Optimizacion extrema)
add_compile_options(
    -O3 
    -march=native 
    -fopenmp 
    -Wno-deprecated-declarations
)

# 2. Buscar OpenMP (necesario para hilos paralelos)
find_package(OpenMP REQUIRED)

# 3. Definir el Ejecutable
add_executable(bkh src/bkh.cpp)

# 4. Enlazar Librerias
# Nota: En Linux requiere libsecp256k1-dev y libssl-dev
target_link_libraries(bkh PRIVATE 
    secp256k1 
    crypto 
    OpenMP::OpenMP_CXX
)

# 5. Configuracion Especifica por Sistema
if(WIN32)
    # Libreria de red necesaria solo en Windows
    target_link_libraries(bkh PRIVATE ws2_32)
    
    # --- COPIA DE DLLs PORTABLES (Carpeta win_dll en la raiz) ---
    set(PROJECT_DLL_DIR "${CMAKE_SOURCE_DIR}/win_dll")
    
    set(DLL_LIST
        "libsecp256k1-6.dll"
        "libcrypto-3-x64.dll"
        "libgomp-1.dll"
        "libwinpthread-1.dll"
        "libstdc++-6.dll"
        "libgcc_s_seh-1.dll"
    )

    foreach(DLLNAME ${DLL_LIST})
        set(FULL_DLL_PATH "${PROJECT_DLL_DIR}/${DLLNAME}")
        if(EXISTS "${FULL_DLL_PATH}")
            add_custom_command(TARGET bkh POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${FULL_DLL_PATH}"
                "${CMAKE_BINARY_DIR}/bin/"
                COMMENT "Copiando DLL: ${DLLNAME}"
            )
        endif()
    endforeach()
    message(STATUS "[+] Modo Windows detectado: DLLs de win_dll configuradas.")
else()
    message(STATUS "[+] Modo Linux/Unix detectado.")
endif()

# 6. Directorio de salida y archivo de idioma
# Esto asegura que el binario siempre caiga en /bin dentro de la carpeta de build
set_target_properties(bkh PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

# Copia el lang.ini a la carpeta bin del build actual cada vez que compiles
add_custom_command(TARGET bkh POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${CMAKE_SOURCE_DIR}/src/lang.ini"
    "${CMAKE_BINARY_DIR}/bin/lang.ini"
    COMMENT "Copiando archivo de idioma..."
)

message(STATUS "[+] Configuracion de BitcoinKeyHunter lista")