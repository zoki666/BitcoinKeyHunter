# --- VARIABLES ---
CXX = g++
# Agregamos -std=c++17 para asegurar compatibilidad con las funciones modernas de bkh v6.4
CXXFLAGS = -O3 -march=native -fopenmp -std=c++17
# El orden de LIBS es importante: primero la logica de secp256k1, luego la criptografia base
LIBS = -lsecp256k1 -lcrypto

# Directorios
SRC = src/bkh.cpp
BIN_DIR = bin

# Deteccion de OS para el binario
ifeq ($(OS),Windows_NT)
    TARGET = $(BIN_DIR)/bkh.exe
    # En Windows a veces es necesario enlazar ws2_32 para funciones de red/entropy de openssl
    LIBS += -lws2_32
else
    TARGET = $(BIN_DIR)/bkh
endif

# --- REGLAS ---

all: $(TARGET)

$(TARGET): $(SRC)
	@echo "------------------------------------------"
	@echo " [+] Iniciando compilacion v6.4 (Hybrid)..."
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET) $(LIBS)
	@echo "------------------------------------------"
	@echo " [!] EXITO: $(TARGET) generado."
	@echo " [!] Listo para buscar Legacy y Taproot."
	@echo "------------------------------------------"

clean:
	@rm -rf $(BIN_DIR)/bkh $(BIN_DIR)/bkh.exe
	@echo " [+] Limpieza completada."

help:
	@echo "--- Ayuda del Makefile Hunter ---"
	@echo "Sistema detectado: $(OS)"
	@echo "Compilador:       $(CXX)"
	@echo "Destino:          $(TARGET)"
	@echo "--------------------------"
	@echo "Comandos disponibles:"
	@echo "  make            - Compila el buscador"
	@echo "  make clean      - Borra los binarios"
	@echo "  make help       - Muestra esta info"