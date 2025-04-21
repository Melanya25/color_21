#include "mongoose.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "constants.h" // Предположим, что он содержит пути

static int process_request(struct mg_connection *c, struct mg_http_message *hm) {
    if (!mg_strcmp(hm->uri, mg_str("/"))) {
        // Отправка формы выбора цветов
        char *response = read_file(PATH_COLORS_HTML);
        if (response) {
            mg_http_reply(c, 200, "Content-Type: text/html\r\n", "%s", response);
            free(response);
            return 1; // Успешно обработано
        } else {
            mg_http_reply(c, 500, "", "Ошибка чтения файла colors.html");
            return 0; // Ошибка
        }
    } else if (!mg_strcmp(hm->uri, mg_str("/result")) && !mg_strcasecmp(hm->method, mg_str("POST"))) {
        // Обработка данных формы
        char text_red[4], text_green[4], text_blue[4];
        char bg_red[4], bg_green[4], bg_blue[4];
        char text[512]; // Ограничение на длину текста

        mg_http_get_var(&hm->body, "text_red", text_red, sizeof(text_red));
        mg_http_get_var(&hm->body, "text_green", text_green, sizeof(text_green));
        mg_http_get_var(&hm->body, "text_blue", text_blue, sizeof(text_blue));

        mg_http_get_var(&hm->body, "bg_red", bg_red, sizeof(bg_red));
        mg_http_get_var(&hm->body, "bg_green", bg_green, sizeof(bg_green));
        mg_http_get_var(&hm->body, "bg_blue", bg_blue, sizeof(bg_blue));

        mg_http_get_var(&hm->body, "text", text, sizeof(text));

        // Загрузка шаблона result.html
        char *result_html = read_file(PATH_RESULT_HTML);
        if (!result_html) {
            mg_http_reply(c, 500, "", "Ошибка чтения файла result.html");
            return 0;
        }

        // Подготовка ответа (замена переменных)
        char response[2048]; // Размер буфера нужно подбирать
        snprintf(response, sizeof(response), result_html,
                 text_red, text_green, text_blue,
                 bg_red, bg_green, bg_blue,
                 text);
        free(result_html);

        mg_http_reply(c, 200, "Content-Type: text/html\r\n", "%s", response);
        return 1;
    }
    // Если URI не распознан, возвращаем 404
    mg_http_reply(c, 404, "", "Страница не найдена");
    return 0;
}

static void fn(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  if (ev == MG_EV_HTTP_MSG) {
    process_request(c, (struct mg_http_message *) ev_data);
  }
  (void) fn_data;
}

int main(void) {
  struct mg_mgr mgr;
  mg_mgr_init(&mgr);
  mg_http_listen(&mgr, "http://localhost:8000", fn, &mgr); // Измените порт при необходимости
  printf("Сервер запущен на порту 8000\n");

  for (;;) mg_mgr_poll(&mgr, 1000);
  mg_mgr_free(&mgr);

  return 0;
}
