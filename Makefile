CC = gcc
CFLAGS = -Wall -g

# Путь к библиотеке Mongoose (скопируйте файлы mongoose.c и mongoose.h в папку mongoose)
MONGOOSE_DIR = mongoose   #  <--  Проверьте, что путь верен

# Исполняемый файл
TARGET = server

# Все .c файлы, которые нужно скомпилировать
SOURCES = main.c input/input.c $(MONGOOSE_DIR)/mongoose.c

# Объектные файлы
OBJECTS = $(SOURCES:.c=.o)

# Флаги линковщика (в данном случае не нужны дополнительные библиотеки)
LDFLAGS =

all: $(TARGET)

$(TARGET): $(OBJECTS)
    $(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS) $(LDFLAGS)

%.o: %.c
    $(CC) $(CFLAGS) -c $< -o $@

clean:
    rm -f $(OBJECTS) $(TARGET)

run: $(TARGET)
    ./$(TARGET)
