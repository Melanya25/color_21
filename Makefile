# Замените текущее содержимое на:
CC = gcc
CFLAGS = -I./mongoose -I./src
SRC = src/main.c src/input.c mongoose/mongoose.c
TARGET = server

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f $(TARGET)

.PHONY: all clean
