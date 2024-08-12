CC := gcc
CFLAGS := -Wall -Wextra -std=c99 -O3 `pkg-config --cflags SDL2_image`
LDLIBS = `pkg-config --libs SDL2_image` -lm


SRC_DIR := src
INC_DIR := include
DEST_DIR := build
TARGET := filters

SRC := $(wildcard $(SRC_DIR)/*.c)
OBJ := $(patsubst $(SRC_DIR)/%.c,$(DEST_DIR)/%.o,$(SRC))

all: $(DEST_DIR)/$(TARGET)

$(DEST_DIR)/$(TARGET): $(OBJ)
	mkdir -p $(DEST_DIR)
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

$(DEST_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(DEST_DIR)
	$(CC) $(CFLAGS) -I$(INC_DIR) -c $< -o $@

clean:
	rm -rf $(DEST_DIR)
	rm -f *.png

.PHONY: all clean
