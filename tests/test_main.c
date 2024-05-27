#include <assert.h>
#include "mpesa.h"

void test_init_string() {
    struct string s;
    init_string(&s);
    assert(s.len == 0);
    assert(s.ptr != NULL);
    assert(s.ptr[0] == '\0');
    free(s.ptr);
}

// int main(void) {
//     test_init_string();
//     printf("[+] All tests passed.\n");
//     return (0);
// }
