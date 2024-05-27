#include <stdio.h>
#include "mpesa.h"

int main(void) {
    struct string token;
    init_string(&token);

    char *consumer_key = "************************************************";
    char *consumer_secret = "****************************************************************";
    
    get_oauth_token(consumer_key, consumer_secret, &token);

    if (token.ptr) {
        initiate_stk_push(token.ptr);
        free(token.ptr);
    }

    return (0);
}