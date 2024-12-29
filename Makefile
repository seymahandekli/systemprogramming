CC = gcc
CFLAGS = -Wall -g -Iinclude
TARGET = bin/file_manager
SRCS = src/main.c src/file_operations.c src/directory_ops.c src/permissions.c src/logger.c
OBJS = $(patsubst src/%.c,build/%.o,$(SRCS))

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

build/%.o: src/%.c
	mkdir -p build
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf build $(TARGET)
