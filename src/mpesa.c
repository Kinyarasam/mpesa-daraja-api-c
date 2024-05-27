#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include "mpesa.h"

void init_string(struct string *s) {
    s->len = 0;
    s->ptr = malloc(s->len + 1);
    if (s->ptr == NULL) {
        fprintf(stderr, "malloc() failed\n");
        exit(EXIT_FAILURE);
    }
    s->ptr[0] = '\0';
}

size_t writefunc(void *ptr, size_t size, size_t nmemb, struct string *s) {
    size_t new_len = s->len + size * nmemb;
    s->ptr = realloc(s->ptr, new_len + 1);
    if (s->ptr == NULL) {
        fprintf(stderr, "realloc() failed");
        exit(EXIT_FAILURE);
    }
    memcpy(s->ptr + s->len, ptr, size * nmemb);
    s->ptr[new_len] = '\0';
    s->len = new_len;

    return size * nmemb;
}

void get_oauth_token(char *consumer_key, char *consumer_secret, struct string *token) {
    CURL *curl;
    CURLcode res;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    if (curl) {
        struct curl_slist *chunk = NULL;
        chunk = curl_slist_append(chunk, "Content-Type: application/json");

        curl_easy_setopt(curl, CURLOPT_URL, "https://sandbox.safaricom.co.ke/oauth/v1/generate?grant_type=client_credentials");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);
        curl_easy_setopt(curl, CURLOPT_USERNAME, consumer_key);
        curl_easy_setopt(curl, CURLOPT_PASSWORD, consumer_secret);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, token);

        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }

        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();
}

void initiate_stk_push(const char *token) {
    CURL *curl;
    CURLcode res;
    struct string s;
    init_string(&s);

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    if (curl) {
        struct curl_slist *chunk = NULL;
        char auth_header[100];
        snprintf(auth_header, 100, "Authorization: Bearer %s", token);
        chunk = curl_slist_append(chunk, auth_header);

        const char *post_data = "{\"BusinessShortCode\": \"\"}";

        curl_easy_setopt(curl, CURLOPT_URL, "https://sandbox.safaricom.co.ke/mpesa/stkpush/v1/processrequest");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_data);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc),
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);

        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        } else {
            printf("%s\n", s.ptr);
        }
        curl_easy_cleanup(curl);
    }
    free(s.ptr);
    curl_global_cleanup();
}