cat > Makefile << 'EOF'
CC = gcc
CFLAGS = -I. -I./src -I./src/input
SRC = $(wildcard src/*.c) $(wildcard src/input/*.c) mongoose.c
TARGET = server

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f $(TARGET)

.PHONY: all clean
EOF
