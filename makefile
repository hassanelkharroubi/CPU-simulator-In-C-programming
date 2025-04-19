# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -g -Iinclude

# Directories
SRC_DIR = src
INC_DIR = include
TEST_DIR = tests

# Source files
SRCS = $(SRC_DIR)/hashmap.c $(SRC_DIR)/memory_handler.c
OBJS = $(SRCS:.c=.o)

# Test files
TEST_SRCS = $(TEST_DIR)/test_memory.c
TEST_OBJS = $(TEST_SRCS:.c=.o)

# Target executable
TARGET = test_runner

# Build rules
all: $(TARGET)

$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TEST_DIR)/%.o: $(TEST_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJS) $(TEST_OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(TEST_OBJS) -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(SRC_DIR)/*.o $(TEST_DIR)/*.o $(TARGET)
