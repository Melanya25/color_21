// Защита от многократного включения заголовочного файла
#ifndef CONSTANTS_H
#define CONSTANTS_H

// Определение директорий для хранения файлов

// Директория для HTML-шаблонов
#define DIR_TEMPLATES "./templates/"
// Директория для CSS-стилей
#define DIR_CSS       "./css/"

// Путь к HTML-форме для выбора цветов
#define PATH_COLOR_FORM_HTML   DIR_TEMPLATES "color_form.html"
// Путь к HTML-шаблону результата (после отправки формы)
#define PATH_COLOR_RESULT_HTML DIR_TEMPLATES "color_result.html"
// Путь к файлу CSS-стилей
#define PATH_CSS_STYLES      DIR_CSS       "styles.css"

// Заголовок для HTML-контента
#define CONTENT_TYPE_HTML "Content-Type: text/html; charset=utf-8\r\n"
// Заголовок для CSS-контента
#define CONTENT_TYPE_CSS  "Content-Type: text/css; charset=utf-8\r\n"

// Конец защиты от многократного включения
#endif
