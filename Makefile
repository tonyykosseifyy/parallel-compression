CC=gcc
CFLAGS=-Iinclude -pthread  # Moved -pthread here to apply for all compilations
SRC_DIR=src
BIN_DIR=bin
OBJ_DIR=obj

# Define source files and object files
SRCS=$(SRC_DIR)/main.c $(SRC_DIR)/sequential.c $(SRC_DIR)/utils.c $(SRC_DIR)/parallel_2.c
OBJS=$(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Target executable name
TARGET=$(BIN_DIR)/program

# Default target
all: $(TARGET)

# Link object files into a binary
$(TARGET): $(OBJS)
	$(CC) $^ -o $@ -pthread 

# Compile source files into object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up compiled files
clean:
	rm -rf $(BIN_DIR)/* $(OBJ_DIR)/*

.PHONY: all clean
