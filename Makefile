# Определение компилятора
CC = gcc

# Флаги компиляции:
# -I. - включаем текущую директорию для поиска заголовков
# -I./mongoose - включаем директорию mongoose
# -Wall - все предупреждения
# -Wextra - дополнительные предупреждения
# -Werror - трактовать предупреждения как ошибки
# -g - отладочная информация
CFLAGS = -I. -I./mongoose -Wall -Wextra -Werror -g

# Список исходных файлов
SRC = src/main.c src/input/input.c mongoose/mongoose.c

# Объектные файлы (автоматическая замена .c на .o)
OBJ = $(SRC:.c=.o)

# Имя исполняемого файла
TARGET = server

# Основная цель по умолчанию
all: $(TARGET)

# Правило сборки основного приложения
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^
	@echo "Сервер успешно собран!"

# Общее правило для компиляции .c файлов в .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Цель для запуска сервера (после сборки)
run: all
	@echo "Запуск сервера на http://localhost:8081"
	@./$(TARGET)

# Цель для очистки собранных файлов
clean:
	rm -f $(OBJ) $(TARGET)
	@echo "Очистка завершена"

# Цель для проверки структуры проекта
check:
	@echo "Проверка структуры проекта..."
	@test -f src/main.c || echo "Ошибка: отсутствует src/main.c"
	@test -f src/input/input.c || echo "Ошибка: отсутствует src/input/input.c"
	@test -f src/input/input.h || echo "Ошибка: отсутствует src/input/input.h"
	@test -f src/constants.h || echo "Ошибка: отсутствует src/constants.h"
	@test -f src/templates/color_form.html || echo "Ошибка: отсутствует src/templates/color_form.html"
	@test -f src/templates/color_result.html || echo "Ошибка: отсутствует src/templates/color_result.html"
	@test -f src/css/styles.css || echo "Ошибка: отсутствует src/css/styles.css"
	@echo "Проверка завершена"

# Цель для форматирования кода (требуется clang-format)
format:
	find src/ -name '*.c' -o -name '*.h' | xargs clang-format -i
	@echo "Форматирование завершено"

# Файлы, от которых не зависят другие правила
.PHONY: all clean run check format
