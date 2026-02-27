CC = gcc
CFLAGS = -Wall -Wextra -g -Iinclude

SRC_DIR = src
INC_DIR = include
OBJ_DIR = obj

SRCS = $(wildcard $(SRC_DIR)/*.c) main.c
OBJS= $(patsubst %.c, $(OBJ_DIR)/%.o, $(SRCS))

TARGET = scott

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o:%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(TARGET) compile_commands.json

.PHONY: all clean

