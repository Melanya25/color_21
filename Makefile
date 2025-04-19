cat > Makefile << 'EOF'
CC = gcc
CFLAGS = -I. -I./src -I./src/input
SRC = src/main.c src/input/input.c mongoose.c
TARGET = server

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f $(TARGET)

.PHONY: all clean
EOF
