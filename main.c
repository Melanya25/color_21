cat > main.c << 'EOF'
#include "mongoose/mongoose.h"
#include "input/input.h"
#include "constants.h"
#include <stdlib.h>
#include <stdbool.h>

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

    if (!mg_strcmp(hm->url, mg_str("/login")) && !mg_strcasecmp(hm->method, mg_str("POST"))) {
        char username[100], password[100];
        const char *expected_user = getenv("LOGIN_USER");
        const char *expected_pass = getenv("LOGIN_PASS");

        if (!expected_user || !expected_pass) {
            error_code = ERR_MISSING_ENV;
        } else {
            mg_http_get_var(&hm->body, "username", username, sizeof(username));
            mg_http_get_var(&hm->body, "password", password, sizeof(password));

            if (!strcmp(username, expected_user) && !strcmp(password, expected_pass)) {
                response = read_file(PATH_COLOR_HTML);
            } else {
                response = read_file(PATH_ERROR_HTML);
                error_code = ERR_INVALID_CREDENTIALS;
            }
        }
    }
    else if (!mg_strcmp(hm->url, mg_str("/color")) && !mg_strcasecmp(hm->method, mg_str("POST"))) {
        char text[500], bg_r[4], bg_g[4], bg_b[4], text_r[4], text_g[4], text_b[4];
        
        mg_http_get_var(&hm->body, "text", text, sizeof(text));
        mg_http_get_var(&hm->body, "bg_r", bg_r, sizeof(bg_r));
        mg_http_get_var(&hm->body, "bg_g", bg_g, sizeof(bg_g));
        mg_http_get_var(&hm->body, "bg_b", bg_b, sizeof(bg_b));
        mg_http_get_var(&hm->body, "text_r", text_r, sizeof(text_r));
        mg_http_get_var(&hm->body, "text_g", text_g, sizeof(text_g));
        mg_http_get_var(&hm->body, "text_b", text_b, sizeof(text_b));

        char *html_template = read_file(PATH_SUCCESS_HTML);
        if (html_template) {
            response = malloc(strlen(html_template) + 500);
            if (response) {
                sprintf(response, html_template, text, bg_r, bg_g, bg_b, text_r, text_g, text_b);
                free(html_template);
            }
        }
    }
    else if (!mg_strcmp(hm->url, mg_str("/styles.css"))) {
        response = read_file(PATH_CSS_STYLES);
    }
    else {
        response = read_file(PATH_LOGIN_HTML);
    }

    if (response) {
        status_code = 200;
        ctype = CONTENT_TYPE_HTML;
        if (!mg_strcmp(hm->url, mg_str("/styles.css"))) {
            ctype = CONTENT_TYPE_CSS;
        }
        error_code = ERR_OK;
    }

    if (error_code == ERR_OK) {
        mg_http_reply(c, status_code, ctype, "%s", response);
    } else {
        mg_http_reply(c, 500, "", "", "");
    }

    free(response);
    return error_code;
}

static void main_fun(struct mg_connection *c, int ev, void *ev_data) {
    if (ev == MG_EV_HTTP_MSG) {
        struct mg_http_message *hm = (struct mg_http_message *) ev_data;
        process_request(c, hm);
    }
}

int main(void) {
    const char *server_address = "http://localhost:8081";
    struct mg_mgr mgr;
    mg_mgr_init(&mgr);
    mg_http_listen(&mgr, server_address, main_fun, NULL);
    printf("Server started on %s\n", server_address);
    for (;;) mg_mgr_poll(&mgr, 1000);
    mg_mgr_free(&mgr);
    return 0;
}
EOF
