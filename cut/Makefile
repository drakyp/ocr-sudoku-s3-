CC := gcc
CFLAGS := -Wall -Wextra -std=c99 -O3

SRC_DIR := src
INC_DIR := include
DEST_DIR := build
TARGET := cut

SRC := $(wildcard $(SRC_DIR)/*.c)
OBJ := $(patsubst $(SRC_DIR)/%.c,$(DEST_DIR)/%.o,$(SRC))

all: $(DEST_DIR)/$(TARGET)

$(DEST_DIR)/$(TARGET): $(OBJ)
	mkdir -p $(DEST_DIR)
	$(CC) $(CFLAGS) -o $@ $^ -lSDL2 -lSDL2_image -lm

$(DEST_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(DEST_DIR)
	$(CC) $(CFLAGS) -I$(INC_DIR) -c $< -o $@ -lSDL2 -lSDL2_image -lm

clean:
	rm -rf $(DEST_DIR)
	rm -rf output

.PHONY: all clean
