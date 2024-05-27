#ifndef __MPESA_H__
#define __MPESA_H__

#include <stdlib.h>
#include <curl/curl.h>

struct string {
    char *ptr;
    size_t len;
};

void init_string(struct string *s);
size_t writefunc(void *ptr, size_t size, size_t nmemb, struct string *s);
void get_oauth_token(char *consumer_key, char *consumer_secret, struct string *token);
void initiate_stk_push(const char *token);

#endif /** __MPESA_H__ */
