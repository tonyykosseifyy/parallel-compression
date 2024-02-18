CC=gcc
CFLAGS=-Iinclude 

SRC_DIR=src
BIN_DIR=bin
OBJ_DIR=obj

# Specify source files
SRCS=$(SRC_DIR)/main.c $(SRC_DIR)/sequential.c $(SRC_DIR)/utils.c $(SRC_DIR)/parallel_2.c $(SRC_DIR)/parallel_N.c

OBJS=$(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Target executable name
TARGET=$(BIN_DIR)/program

# Default target
all: $(TARGET)

# Link object files into a binary
# Add -pthread here if other parts of your program still use pthreads
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
