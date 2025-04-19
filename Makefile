CC = gcc
CFLAGS = -Wall -Wextra -std=c11
SRC = main.c mongoose/mongoose.c input/input.c
TARGET = server

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(TARGET)

.PHONY: all clean
