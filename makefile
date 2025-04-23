# Makefile

CC      := gcc
CFLAGS  := -Wall -Wextra -g -Iinclude

SRC_DIR  := src
TEST_DIR := tests
OBJ_DIR  := obj
BIN_DIR  := bin

# All .c under src/ and all test_*.c under tests/
SRC_FILES   := $(wildcard $(SRC_DIR)/*.c)
TEST_SRCS   := $(wildcard $(TEST_DIR)/test_*.c)

# Map to corresponding .o in obj/
OBJ_FILES   := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC_FILES))
TEST_OBJS   := $(patsubst $(TEST_DIR)/%.c,$(OBJ_DIR)/%.o,$(TEST_SRCS))

# One executable per test, named bin/<testname>
TEST_BINS   := $(patsubst $(TEST_DIR)/%.c,$(BIN_DIR)/%,$(TEST_SRCS))

.PHONY: all clean

all: dirs $(TEST_BINS)

# Link each test binary against all src objects + its own test obj
$(BIN_DIR)/%: $(OBJ_DIR)/%.o $(OBJ_FILES)
	$(CC) $(CFLAGS) $^ -o $@

# Compile application sources -> obj/
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | dirs
	$(CC) $(CFLAGS) -c $< -o $@

# Compile test sources -> obj/
$(OBJ_DIR)/%.o: $(TEST_DIR)/%.c | dirs
	$(CC) $(CFLAGS) -c $< -o $@

# Ensure directories exist
dirs:
	mkdir -p $(OBJ_DIR) $(BIN_DIR)

# Clean up
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)
