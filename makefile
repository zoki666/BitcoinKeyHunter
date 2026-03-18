# --- VARIABLES ---
CXX = g++
CXXFLAGS = -O3 -march=native -fopenmp -std=c++17 -Wno-deprecated-declarations
LIBS = -lsecp256k1 -lcrypto

# Directorios y Archivos
SRC = src/bkh.cpp
LANG_SRC = src\lang.ini
BIN_DIR = bin

# Deteccion de OS y ajuste de comandos
ifeq ($(OS),Windows_NT)
    TARGET = $(BIN_DIR)\bkh.exe
    LANG_DEST = $(BIN_DIR)\lang.ini
    LIBS += -lws2_32
    RM = del /Q /F
    CP = copy /Y
    MKDIR = if not exist $(BIN_DIR) mkdir $(BIN_DIR)
    CLEAN_CMD = $(RM) "$(TARGET)" "$(LANG_DEST)" 2>nul || exit 0
else
    TARGET = $(BIN_DIR)/bkh
    LANG_DEST = $(BIN_DIR)/lang.ini
    RM = rm -rf
    CP = cp
    MKDIR = mkdir -p $(BIN_DIR)
    CLEAN_CMD = $(RM) $(TARGET) $(LANG_DEST)
endif

# --- REGLAS ---

all: $(TARGET)

$(TARGET): $(SRC)
	@echo ------------------------------------------
	@echo  [+] Iniciando compilacion v6.4...
	@$(MKDIR)
	@$(CXX) $(CXXFLAGS) $(SRC) -o "$(TARGET)" $(LIBS)
	@echo  [+] Intentando copiar: $(LANG_SRC) a $(LANG_DEST)
	@$(CP) "$(LANG_SRC)" "$(LANG_DEST)" >nul
	@echo ------------------------------------------
	@echo  [!] EXITO: $(TARGET) generado y configurado.
	@echo ------------------------------------------

clean:
	@echo  [+] Limpiando binarios...
	@$(CLEAN_CMD)
	@echo  [+] Limpieza completada.