CXX = g++
CXXFLAGS = -std=c++17 -O2 -pthread -Iinclude

BIN = bin
BUILD = build
SRC = src
INC = include

# -----------------------------
# Original snapshot tests
# -----------------------------
ORIG_TARGET = $(BIN)/snapshot_tests
ORIG_SRCS = $(SRC)/main.cpp \
            $(SRC)/snapshot_consistency.cpp \
            $(SRC)/snapshot_isolation.cpp \
            $(SRC)/snapshot_stress.cpp \
            $(SRC)/FairShareMutex.cpp
ORIG_OBJS = $(patsubst $(SRC)/%.cpp,$(BUILD)/%.o,$(ORIG_SRCS))

# -----------------------------
# Upgraded KVStore tests
# -----------------------------
UPG_TARGET = $(BIN)/upgraded_snapshot_tests
UPG_SRCS = $(SRC)/upgraded_main.cpp \
           $(SRC)/snapshot_consistency.cpp \
           $(SRC)/snapshot_isolation.cpp \
           $(SRC)/snapshot_stress.cpp \
           $(SRC)/FairShareMutex.cpp \
           $(SRC)/lock_upgrade.cpp \
           $(SRC)/batch_write.cpp
UPG_OBJS = $(patsubst $(SRC)/%.cpp,$(BUILD)/%.o,$(UPG_SRCS))

# -----------------------------
# Default target
# -----------------------------
all: $(ORIG_TARGET) $(UPG_TARGET)

# -----------------------------
# Link original snapshot tests
# -----------------------------
$(ORIG_TARGET): $(ORIG_OBJS)
	mkdir -p $(BIN)
	$(CXX) $(CXXFLAGS) -o $@ $^

# -----------------------------
# Link upgraded snapshot tests
# -----------------------------
$(UPG_TARGET): $(UPG_OBJS)
	mkdir -p $(BIN)
	$(CXX) $(CXXFLAGS) -o $@ $^

# -----------------------------
# Compile source files to objects
# -----------------------------
$(BUILD)/%.o: $(SRC)/%.cpp
	mkdir -p $(BUILD)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# -----------------------------
# Clean build and binary folders
# -----------------------------
clean:
	rm -rf $(BUILD) $(BIN)

.PHONY: all clean
