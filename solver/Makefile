CC := gcc
CFLAGS := -Wall -Wextra -std=c99 -O3

SRC_DIR := src
INC_DIR := include
DEST_DIR := build
TARGET := solver

SRC := $(wildcard $(SRC_DIR)/*.c)
OBJ := $(patsubst $(SRC_DIR)/%.c,$(DEST_DIR)/%.o,$(SRC))

all: $(DEST_DIR)/$(TARGET)

$(DEST_DIR)/$(TARGET): $(OBJ)
	mkdir -p $(DEST_DIR)
	$(CC) $(CFLAGS) -o $@ $^

$(DEST_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(DEST_DIR)
	$(CC) $(CFLAGS) -I$(INC_DIR) -c $< -o $@

clean:
	rm -rf $(DEST_DIR)

.PHONY: all clean
