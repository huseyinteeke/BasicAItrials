# Derleyici ayarları
CXX      := C:/msys64/ucrt64/bin/g++.exe
CXXFLAGS := -Wall -Wextra -Iinc -O2 -std=c++23 

# Dosya yolları
BIN_DIR := bin
SRC_DIR := src

# Kaynak ve obje dosyaları
SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(BIN_DIR)/%.o,$(SRCS))
TARGET := $(BIN_DIR)/app.exe

# İlk hedef
all: $(TARGET)

# Linkleme
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Obje dosyalarını oluşturma
$(BIN_DIR)/%.o: $(SRC_DIR)/%.cpp
	if not exist "$(BIN_DIR)" mkdir "$(BIN_DIR)"
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Windows için temizleme komutu
clean:
	if exist "$(BIN_DIR)" del /f /s /q "$(BIN_DIR)\*.*"

.PHONY: all clean