CC = gcc
CFLAGS = -Wall -Wextra -g
#CFLAGS = -Wall -Wextra -O2  # Для production (оптимизация)

SOURCES = main.c input/input.c mongoose/mongoose.c
HEADERS = input/input.h constants/constants.h mongoose/mongoose.h
OBJECTS = $(SOURCES:.c=.o)
EXECUTABLE = server

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)
