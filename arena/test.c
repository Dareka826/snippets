#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <math.h>
#include "../short_types.h"
#include "arena.h"


int main() {

    // [...]

    u32 x = 1234;

    {
        Arena *a = create_arenan("a");

        char *buf = NULL;
        {
            const char *text = "Number:";
            size_t bufsize = strlen(text) + 1 + ((size_t) floor(log10(UINT32_MAX)) + 1) + 1;

            buf = arena_xalloc0(a, bufsize);
            snprintf(buf, bufsize, "%s %d", text, x);
        }

        char *str1 = arena_xalloc0(a, 10);
        strcpy(str1, "test_12");

        printf("%s\n", str1);

        char *str2 = arena_xalloc0(a, 20);
        memset(str2, '#', 19);

        printf("%s\n", str2);

        printf("%s\n", buf);

        nfree_arena(a);
    }

    // [...]

    return 0;
}
