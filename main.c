#include "mongoose.h"
#include "input.h"
#include "constants.h"
#include <stdlib.h>
#include <string.h>

enum {
    ERR_OK = 0,
    ERR_MISSING_ENV = 1,
    ERR_INVALID_CREDENTIALS = 2,
    ERR_FILE_NOT_FOUND = 3
};

static int process_request(struct mg_connection *c, struct mg_http_message *hm) {
    int status_code = 500;
    const char *ctype = "";
    char *response = NULL;
    int error_code = ERR_FILE_NOT_FOUND;

    if (mg_strcmp(hm->url, mg_str("/login")) == 0 && mg_strcasecmp(hm->method, mg_str("POST")) == 0) {
        // Обработка авторизации (как в оригинале)
    } 
    else if (mg_strcmp(hm->url, mg_str("/color")) == 0 && mg_strcasecmp(hm->method, mg_str("POST")) == 0) {
        // Обработка формы цвета
        char bg_red[4], bg_green[4], bg_blue[4], text_red[4], text_green[4], text_blue[4], content[256];
        
        mg_http_get_var(&hm->body, "bg_red", bg_red, sizeof(bg_red));
        mg_http_get_var(&hm->body, "bg_green", bg_green, sizeof(bg_green));
        mg_http_get_var(&hm->body, "bg_blue", bg_blue, sizeof(bg_blue));
        mg_http_get_var(&hm->body, "text_red", text_red, sizeof(text_red));
        mg_http_get_var(&hm->body, "text_green", text_green, sizeof(text_green));
        mg_http_get_var(&hm->body, "text_blue", text_blue, sizeof(text_blue));
        mg_http_get_var(&hm->body, "content", content, sizeof(content));

        char *color_template = read_file(PATH_COLOR_HTML);
        if (color_template) {
            char *dynamic_response = malloc(strlen(color_template) + 512);
            sprintf(dynamic_response, color_template, 
                    bg_red, bg_green, bg_blue,
                    text_red, text_green, text_blue,
                    content);
            
            status_code = 200;
            ctype = CONTENT_TYPE_HTML;
            error_code = ERR_OK;
            response = dynamic_response;
            free(color_template);
        }
    }
    else if (mg_strcmp(hm->url, mg_str("/styles.css")) == 0) {
        response = read_file(PATH_CSS_STYLES);
        if (response) {
            status_code = 200;
            ctype = CONTENT_TYPE_CSS;
            error_code = ERR_OK;
        }
    } else {
        response = read_file(PATH_LOGIN_HTML);
        if (response) {
            status_code = 200;
            ctype = CONTENT_TYPE_HTML;
            error_code = ERR_OK;
        }
    }

    if (error_code == ERR_OK) {
        mg_http_reply(c, status_code, ctype, "%s", response);
    } else {
        mg_http_reply(c, 500, "", "", "");
    }

    free(response);
    return error_code;
}

// Остальной код как в оригинале...
