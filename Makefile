CC = gcc
CFLAGS = -Wall -g
TARGET = bin/file_manager
SRCS = src/main.c src/file_operations.c src/directory_ops.c src/permissions.c src/logger.c
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
