CC = gcc
CFLAGS = -Wall -g -I./mongoose

TARGET = server
SOURCES = main.c input/input.c mongoose/mongoose.c

OBJECTS = $(SOURCES:.c=.o)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJECTS)
