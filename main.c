#include "../mongoose/mongoose.h"
#include "input.h"
#include "constants.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// [NEW] Color response generator
static char *generate_color_response(const char *text, 
                                   const char *bg_red, const char *bg_green, const char *bg_blue,
                                   const char *text_red, const char *text_green, const char *text_blue) {
    char *html = malloc(1024);
    snprintf(html, 1024,
        "<!DOCTYPE html><html><head><title>Color Result</title>"
        "<style>body{background-color:rgb(%s,%s,%s);color:rgb(%s,%s,%s);"
        "display:flex;justify-content:center;align-items:center;height:100vh;"
        "font-size:2em;}</style></head><body>%s</body></html>",
        bg_red, bg_green, bg_blue, text_red, text_green, text_blue, text);
    return html;
}

static int process_request(struct mg_connection *c, struct mg_http_message *hm) {
    // ... существующий код авторизации ...

    // [NEW] Color configuration handler
    else if (mg_strcmp(hm->url, mg_str("/color")) == 0) {
        if (mg_strcasecmp(hm->method, mg_str("POST")) == 0) {
            char text[100], bg_red[4], bg_green[4], bg_blue[4];
            char text_red[4], text_green[4], text_blue[4];
            
            mg_http_get_var(&hm->body, "text", text, sizeof(text));
            mg_http_get_var(&hm->body, "bg_red", bg_red, sizeof(bg_red));
            // ... аналогично для других полей ...
            
            char *response = generate_color_response(text, bg_red, bg_green, bg_blue,
                                                   text_red, text_green, text_blue);
            mg_http_reply(c, 200, CONTENT_TYPE_HTML, "%s", response);
            free(response);
            return ERR_OK;
        } else {
            // GET request - show color form
            char *response = read_file(PATH_COLOR_HTML);
            if (response) {
                mg_http_reply(c, 200, CONTENT_TYPE_HTML, "%s", response);
                free(response);
                return ERR_OK;
            }
        }
    }
    
    // ... остальной существующий код ...
}
