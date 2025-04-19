// Добавляем новые константы в constants.h
#define PATH_COLOR_FORM_HTML    DIR_TEMPLATES "color_form.html"
#define PATH_COLOR_RESULT_HTML  DIR_TEMPLATES "color_result.html"

// Добавляем новый обработчик в функцию process_request
else if (!mg_strcmp(hm->url, mg_str("/color_form"))) {
    response = read_file(PATH_COLOR_FORM_HTML);
    if (response) {
        status_code = 200;
        ctype = CONTENT_TYPE_HTML;
        error_code = ERR_OK;
    }
}
else if (!mg_strcmp(hm->url, mg_str("/color")) &&
         !mg_strcasecmp(hm->method, mg_str("POST"))) {
    char text[256], bg_r[4], bg_g[4], bg_b[4], text_r[4], text_g[4], text_b[4];
    
    mg_http_get_var(&hm->body, "text", text, sizeof(text));
    mg_http_get_var(&hm->body, "bg_r", bg_r, sizeof(bg_r));
    mg_http_get_var(&hm->body, "bg_g", bg_g, sizeof(bg_g));
    mg_http_get_var(&hm->body, "bg_b", bg_b, sizeof(bg_b));
    mg_http_get_var(&hm->body, "text_r", text_r, sizeof(text_r));
    mg_http_get_var(&hm->body, "text_g", text_g, sizeof(text_g));
    mg_http_get_var(&hm->body, "text_b", text_b, sizeof(text_b));
    
    char *template = read_file(PATH_COLOR_RESULT_HTML);
    if (template) {
        // Заменяем плейсхолдеры на реальные значения
        char *result = malloc(strlen(template) + 512);
        if (result) {
            snprintf(result, strlen(template) + 512, template,
                    bg_r, bg_g, bg_b,
                    text_r, text_g, text_b,
                    text);
            
            status_code = 200;
            ctype = CONTENT_TYPE_HTML;
            error_code = ERR_OK;
            response = result;
        }
        free(template);
    }
}
