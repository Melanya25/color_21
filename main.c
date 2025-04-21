#include "./mongoose/mongoose.h"  // Библиотека Mongoose для веб-сервера
#include "./input/input.h"        // Пользовательские функции ввода
#include "./constants/constants.h" // Пользовательские константы
#include <stdio.h>                
#include <stdlib.h>               
#include <string.h>               // Функции для работы со строками

// Функция обработки HTTP-запросов
static int process_request(struct mg_connection *c, struct mg_http_message *hm) {
    int status_code = 500; // Код состояния HTTP 
    const char *ctype = ""; // Тип контента для ответа
    char *response = NULL; // Буфер для хранения ответа
    
    // Буферы для хранения цветовых компонент из формы
    char bg_red[4], bg_green[4], bg_blue[4];    // Цвета фона (RGB)
    char text_red[4], text_green[4], text_blue[4]; // Цвета текста (RGB)

    // Обработка POST-запроса на /set_colors (отправка данных формы)
    if (!mg_strcmp(hm->uri, mg_str("/set_colors")) && !mg_strcasecmp(hm->method, mg_str("POST"))) {
        // Извлечение значений из тела POST-запроса
        mg_http_get_var(&hm->body, "bg_red", bg_red, sizeof(bg_red));
        mg_http_get_var(&hm->body, "bg_green", bg_green, sizeof(bg_green));
        mg_http_get_var(&hm->body, "bg_blue", bg_blue, sizeof(bg_blue));
        mg_http_get_var(&hm->body, "text_red", text_red, sizeof(text_red));
        mg_http_get_var(&hm->body, "text_green", text_green, sizeof(text_green));
        mg_http_get_var(&hm->body, "text_blue", text_blue, sizeof(text_blue));

        // Чтение HTML-шаблона для ответа
        response = read_file(PATH_COLOR_RESULT_HTML);

        if (response) {
            // Создание буфера для модифицированного HTML (с добавлением места для замен)
            char *result = malloc(strlen(response) + 512);
            if (result) {
                strcpy(result, response);
                
                // Замена плейсхолдеров в HTML на реальные значения цветов
                
                // Замена цвета фона (красный компонент)
                char *bg_red_placeholder = strstr(result, "{{bg_red}}");
                if (bg_red_placeholder) {
                    memcpy(bg_red_placeholder, bg_red, strlen(bg_red));
                }
                
                // Замена цвета фона (зеленый компонент)
                char *bg_green_placeholder = strstr(result, "{{bg_green}}");
                if (bg_green_placeholder) {
                    memcpy(bg_green_placeholder, bg_green, strlen(bg_green));
                }
                
                // Замена цвета фона (синий компонент)
                char *bg_blue_placeholder = strstr(result, "{{bg_blue}}");
                if (bg_blue_placeholder) {
                    memcpy(bg_blue_placeholder, bg_blue, strlen(bg_blue));
                }
                
                // Замена цвета текста (красный компонент)
                char *text_red_placeholder = strstr(result, "{{text_red}}");
                if (text_red_placeholder) {
                    memcpy(text_red_placeholder, text_red, strlen(text_red));
                }
                
                // Замена цвета текста (зеленый компонент)
                char *text_green_placeholder = strstr(result, "{{text_green}}");
                if (text_green_placeholder) {
                    memcpy(text_green_placeholder, text_green, strlen(text_green));
                }
                
                // Замена цвета текста (синий компонент)
                char *text_blue_placeholder = strstr(result, "{{text_blue}}");
                if (text_blue_placeholder) {
                    memcpy(text_blue_placeholder, text_blue, strlen(text_blue));
                }

                // Успешный ответ с модифицированным HTML
                status_code = 200; // OK
                ctype = CONTENT_TYPE_HTML; // Установка типа контента
                mg_http_reply(c, status_code, ctype, "%s", result);
                free(result); // Освобождение памяти
            } else {
                status_code = 500; // Ошибка выделения памяти
            }
            free(response); // Освобождение памяти
        } else {
            status_code = 404; // Файл не найден
        }

    } 
    // Обработка запроса файла стилей
    else if (!mg_strcmp(hm->uri, mg_str("/styles.css"))) {
        response = read_file(PATH_CSS_STYLES);
        if (response) {
            status_code = 200; // OK
            ctype = CONTENT_TYPE_CSS; // Установка типа контента
            mg_http_reply(c, status_code, ctype, "%s", response);
            free(response); // Освобождение памяти
        }
    } 
    // Обработка всех остальных запросов (главная страница)
    else {
        response = read_file(PATH_COLOR_FORM_HTML);
        if (response) {
            status_code = 200; // OK
            ctype = CONTENT_TYPE_HTML; // Установка типа контента
            mg_http_reply(c, status_code, ctype, "%s", response);
            free(response); // Освобождение памяти
        }
    }

    return status_code; // Возврат кода состояния HTTP
}

// Основная callback-функция для обработки событий сервера
static void main_fun(struct mg_connection *c, int ev, void *ev_data) {
    // Обработка события HTTP-запроса
    if (ev == MG_EV_HTTP_MSG) {
        struct mg_http_message *hm = (struct mg_http_message *)ev_data;
        process_request(c, hm); // Передача запроса на обработку
    }
}

// Главная функция программы
int main(void) {
    struct mg_mgr mgr; // Менеджер соединений Mongoose
    mg_mgr_init(&mgr); // Инициализация менеджера
    
    // Создание HTTP-сервера на порту 8081
    mg_http_listen(&mgr, "http://localhost:8081", main_fun, NULL);
    printf("Сервер запущен на http://localhost:8081\n");
    
    // Бесконечный цикл обработки событий
    for (;;) mg_mgr_poll(&mgr, 1000); // Ожидание событий с таймаутом 1000 мс
    
    mg_mgr_free(&mgr); // Освобождение ресурсов (никогда не выполнится в данном коде)
    return 0;
}
