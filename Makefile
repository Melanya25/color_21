cat > Makefile << 'EOF'
CC = gcc
CFLAGS = -I.
SRC = main.c input.c mongoose.c
TARGET = server

all:
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)
EOF
