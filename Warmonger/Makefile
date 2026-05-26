# Derleyici ayarları
CXX      := C:/msys64/ucrt64/bin/g++.exe
CXXFLAGS := -Wall -Wextra -Iinc -O2 -std=c++23 -static-libgcc -static-libstdc++
# Raylib flags for Windows GUI
RAYLIB_FLAGS := -lraylib -lopengl32 -lgdi32 -lwinmm

# Dosya yolları
BIN_DIR := bin
SRC_DIR := src

# Faction ve Merchant modülleri (main içermeyen ortak dosyalar)
COMMON_SRCS := $(SRC_DIR)/dwarves.cpp $(SRC_DIR)/elves.cpp $(SRC_DIR)/faction.cpp $(SRC_DIR)/merchant.cpp $(SRC_DIR)/orcs.cpp
COMMON_OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(BIN_DIR)/%.o,$(COMMON_SRCS))

# GUI ve CLI hedefleri
GUI_MAIN_OBJ := $(BIN_DIR)/main.o
CLI_MAIN_OBJ := $(BIN_DIR)/main_cli.o

TARGET_GUI := $(BIN_DIR)/app.exe
TARGET_CLI := $(BIN_DIR)/app_cli.exe

# İlk hedef (varsayılan olarak GUI derlenir)
all: $(TARGET_GUI)

# CLI hedefi
cli: $(TARGET_CLI)

# GUI Linkleme (Raylib kütüphaneleri ile linkleme)
$(TARGET_GUI): $(COMMON_OBJS) $(GUI_MAIN_OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(RAYLIB_FLAGS)

# CLI Linkleme (Raylib olmadan linkleme)
$(TARGET_CLI): $(COMMON_OBJS) $(CLI_MAIN_OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Obje dosyalarını oluşturma
$(BIN_DIR)/%.o: $(SRC_DIR)/%.cpp
	if not exist "$(BIN_DIR)" mkdir "$(BIN_DIR)"
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Windows için temizleme komutu
clean:
	if exist "$(BIN_DIR)" del /f /s /q "$(BIN_DIR)\*.*"

.PHONY: all cli clean