#include "./mongoose/mongoose.h"
#include "./input/input.h"
#include "./constants/constants.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int process_request(struct mg_connection *c, struct mg_http_message *hm) {
    int status_code = 500;
    const char *ctype = "";
    char *response = NULL;
    char bg_red[4], bg_green[4], bg_blue[4], text_red[4], text_green[4], text_blue[4];

    if (!mg_strcmp(hm->uri, mg_str("/set_colors")) && !mg_strcasecmp(hm->method, mg_str("POST"))) {
        mg_http_get_var(&hm->body, "bg_red", bg_red, sizeof(bg_red));
        mg_http_get_var(&hm->body, "bg_green", bg_green, sizeof(bg_green));
        mg_http_get_var(&hm->body, "bg_blue", bg_blue, sizeof(bg_blue));
        mg_http_get_var(&hm->body, "text_red", text_red, sizeof(text_red));
        mg_http_get_var(&hm->body, "text_green", text_green, sizeof(text_green));
        mg_http_get_var(&hm->body, "text_blue", text_blue, sizeof(text_blue));

        response = read_file(PATH_COLOR_RESULT_HTML);

        if (response) {
            // Замена плейсхолдеров в HTML-шаблоне
            char *result = malloc(strlen(response) + 512);  // Add some extra space
            if (result) {
                strcpy(result, response);
                char *bg_red_placeholder = strstr(result, "{{bg_red}}");
                if (bg_red_placeholder) {
                    memcpy(bg_red_placeholder, bg_red, strlen(bg_red));
                }
                char *bg_green_placeholder = strstr(result, "{{bg_green}}");
                if (bg_green_placeholder) {
                    memcpy(bg_green_placeholder, bg_green, strlen(bg_green));
                }
                char *bg_blue_placeholder = strstr(result, "{{bg_blue}}");
                if (bg_blue_placeholder) {
                    memcpy(bg_blue_placeholder, bg_blue, strlen(bg_blue));
                }
                char *text_red_placeholder = strstr(result, "{{text_red}}");
                if (text_red_placeholder) {
                    memcpy(text_red_placeholder, text_red, strlen(text_red));
                }
                char *text_green_placeholder = strstr(result, "{{text_green}}");
                if (text_green_placeholder) {
                    memcpy(text_green_placeholder, text_green, strlen(text_green));
                }
                char *text_blue_placeholder = strstr(result, "{{text_blue}}");
                if (text_blue_placeholder) {
                    memcpy(text_blue_placeholder, text_blue, strlen(text_blue));
                }

                status_code = 200;
                ctype = CONTENT_TYPE_HTML;
                mg_http_reply(c, status_code, ctype, "%s", result);
                free(result);
            } else {
                status_code = 500;
            }
            free(response);
        } else {
            status_code = 404;  // File not found
        }

    } else if (!mg_strcmp(hm->uri, mg_str("/styles.css"))) {
        response = read_file(PATH_CSS_STYLES);
        if (response) {
            status_code = 200;
            ctype = CONTENT_TYPE_CSS;
            mg_http_reply(c, status_code, ctype, "%s", response);
            free(response);
        }
    } else {
        response = read_file(PATH_COLOR_FORM_HTML);
        if (response) {
            status_code = 200;
            ctype = CONTENT_TYPE_HTML;
            mg_http_reply(c, status_code, ctype, "%s", response);
            free(response);
        }
    }

    return status_code;
}

static void main_fun(struct mg_connection *c, int ev, void *ev_data) {
    if (ev == MG_EV_HTTP_MSG) {
        struct mg_http_message *hm = (struct mg_http_message *)ev_data;
        process_request(c, hm);
    }
}

int main(void) {
    struct mg_mgr mgr;
    mg_mgr_init(&mgr);
    mg_http_listen(&mgr, "http://localhost:8081", main_fun, NULL);
    printf("Сервер запущен на http://localhost:8081\n");
    for (;;) mg_mgr_poll(&mgr, 1000);
    mg_mgr_free(&mgr);
    return 0;
}
