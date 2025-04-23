CC = gcc
CFLAGS = -Wall -Wextra -g -Iinclude

SRC_DIR = src
OBJ_DIR = src
TEST_DIR = tests
BIN_DIR = bin

SRC_FILES = $(SRC_DIR)/hashmap.c $(SRC_DIR)/memory_handler.c $(SRC_DIR)/assembler_parser.c $(SRC_DIR)/parser_result.c $(SRC_DIR)/cpu.c
OBJ_FILES = $(SRC_FILES:.c=.o)

TEST_FILE = $(TEST_DIR)/test_cpu.c
TEST_OBJ = $(TEST_FILE:.c=.o)

TARGET = $(BIN_DIR)/tests

.PHONY: all run clean

all: $(TARGET)

$(TARGET): $(OBJ_FILES) $(TEST_OBJ) | $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $@

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -rf $(OBJ_FILES) $(TEST_OBJ) $(TARGET)
