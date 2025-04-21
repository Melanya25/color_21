#include "./mongoose/mongoose.h"
#include "./input/input.h"
#include "./constants.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static int process_request(struct mg_connection *c, struct mg_http_message *hm) {
    const char *username = getenv("LOGIN_USER");
    const char *password = getenv("LOGIN_PASS");
    char *response = NULL;
    int status_code = 200;
    const char *ctype = CONTENT_TYPE_HTML;

    if (mg_strcmp(hm->uri, mg_str("/login")) == 0 && mg_strcmp(hm->method, mg_str("POST")) == 0) {
        char username_buf[100], password_buf[100];
        mg_http_get_var(&hm->body, "username", username_buf, sizeof(username_buf));
        mg_http_get_var(&hm->body, "password", password_buf, sizeof(password_buf));

        if (username != NULL && password != NULL &&
            strcmp(username_buf, username) == 0 && strcmp(password_buf, password) == 0) {
            response = read_file(PATH_SUCCESS_HTML);
        } else {
            response = read_file(PATH_ERROR_HTML);
        }
    } else if (mg_strcmp(hm->uri, mg_str("/color")) == 0 && mg_strcmp(hm->method, mg_str("POST")) == 0) {
            char color_buf[100];
            mg_http_get_var(&hm->body, "color", color_buf, sizeof(color_buf));
            
            char *color_result_template = read_file(PATH_COLOR_RESULT_HTML);
            if (color_result_template != NULL) {
                response = (char *) malloc(strlen(color_result_template) + 200); // Расширим буфер
                snprintf(response, strlen(color_result_template) + 200, color_result_template, color_buf);
                free(color_result_template);
            } else{
                response = strdup("Error loading color result");
                status_code = 500;
            }
    }
    else if (mg_strcmp(hm->uri, mg_str("/styles.css")) == 0) {
        response = read_file(PATH_CSS_STYLES);
        ctype = CONTENT_TYPE_CSS;
    }
    else {
        response = read_file(PATH_LOGIN_HTML);
    }
    if (response != NULL) {
        mg_http_reply(c, status_code, ctype, "%s", response);
        free(response);
    } else{
         mg_http_reply(c, 500, "", "Internal Server Error");
    }
    return MG_TRUE;
}

static void fn(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
    if (ev == MG_EV_HTTP_MSG) {
        struct mg_http_message *hm = (struct mg_http_message *) ev_data;
        process_request(c, hm);
    }
    (void) fn_data;
}

int main(void) {
    struct mg_mgr mgr;
    mg_mgr_init(&mgr);

    const char *address = "http://0.0.0.0:8000";

    mg_http_listen(&mgr, address, fn, NULL);
    printf("Starting Mongoose web server on %s\n", address);

    for (;;) {
        mg_mgr_poll(&mgr, 1000);
    }
    mg_mgr_free(&mgr);
    return 0;
}
