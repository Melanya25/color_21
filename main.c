#include "./mongoose/mongoose.h"
#include "./input/input.h"
#include "./constants/constants.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int process_request(struct mg_connection *c, struct mg_http_message *hm) {
  char *response = NULL;
  const char *ctype = NULL;
  int status_code = 200;
  char background_color[8] = "#ffffff";  // Значения по умолчанию
  char text_color[8] = "#000000";
  static bool colors_set = false; //флаг установки цветов.

  if (!mg_strcmp(hm->uri, mg_str("/"))) { // Обработка корневого URL
    response = read_file(PATH_LOGIN_HTML); // Загружаем форму
    ctype = CONTENT_TYPE_HTML;
  } else if (!mg_strcmp(hm->uri, mg_str("/styles.css"))) { // Обработка запроса CSS
    response = read_file(PATH_CSS_STYLES);
    ctype = CONTENT_TYPE_CSS;
  } else if (!mg_strcmp(hm->uri, mg_str("/set_colors")) && !mg_strcasecmp(hm->method, mg_str("POST"))) { //Обработка POST-запроса
    mg_http_get_var(&hm->body, "background_color", background_color, sizeof(background_color));
    mg_http_get_var(&hm->body, "text_color", text_color, sizeof(text_color));
    colors_set = true; // Устанавливаем флаг, что цвета установлены
    response = read_file(PATH_SUCCESS_HTML);  // Успех
    ctype = CONTENT_TYPE_HTML;
    printf("Background Color: %s, Text Color: %s\n", background_color, text_color);
  }
   else {
    response = read_file(PATH_LOGIN_HTML);  //  По умолчанию возвращаем форму
    ctype = CONTENT_TYPE_HTML;
  }
    // Добавляем стили в HTML, если цвета были установлены
    if (colors_set && ctype == CONTENT_TYPE_HTML) {

        char *styled_response = NULL;
        size_t styled_response_len;

        // Создаем строку стилей.
        char style_string[256];
        snprintf(style_string, sizeof(style_string), "<style>body { background-color: %s; color: %s; }</style>%s", background_color, text_color, response);
        styled_response_len = strlen(style_string);

        styled_response = (char *)malloc(styled_response_len + 1);
        if (styled_response != NULL) {
             strncpy(styled_response, style_string, styled_response_len);
            styled_response[styled_response_len] = '\0';

        mg_http_reply(c, status_code, ctype, "%s", styled_response);
            free(styled_response); // Освобождаем память после отправки ответа

    } else {
         mg_http_reply(c, status_code, ctype, "%s", response);
    }
    if(response) free(response);
    return 1;

}

static void main_fun(struct mg_connection *c, int ev, void *ev_data) {
  struct mg_http_message *hm = (struct mg_http_message *) ev_data;
  switch (ev) {
    case MG_EV_HTTP_MSG:
      process_request(c, hm);
      break;
    default:
      break;
  }
}

int main(void) {
  struct mg_mgr mgr;
  mg_mgr_init(&mgr);
  mg_http_listen(&mgr, "http://localhost:8000", main_fun, &mgr);
  printf("Сервер запущен на http://localhost:8000\n");
  for (;;) mg_mgr_poll(&mgr, 1000);
  mg_mgr_free(&mgr);
  return 0;
}
