# Define the compiler
CXX = g++

# Define the directories for source, build, and output
SRC_DIR = .
BUILD_DIR = build
BIN_DIR = bin

# Define the source file and output executable
SRC = $(SRC_DIR)/client.cpp
OBJ = $(BUILD_DIR)/client.o
OUT = $(BIN_DIR)/client

# Define any libraries to link
LIBS = -lpthread -lcurl

# Define the default target
all: $(OUT)

# Rule to build the output executable
$(OUT): $(OBJ) | $(BIN_DIR)
	$(CXX) $(OBJ) -o $(OUT) $(LIBS)

# Rule to compile the source file into an object file
$(OBJ): $(SRC) | $(BUILD_DIR)
	$(CXX) -c $(SRC) -o $(OBJ)

# Create build and bin directories if they don't exist
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Rule to clean up generated files
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)
