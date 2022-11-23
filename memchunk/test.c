#include <stdio.h>
#include <stdlib.h>
#include "../short_types.h"
#include "memchunk.h"

void fill(memc * const ptr) {
    u8 * const p = (u8*)(ptr->ptr);

    for (size_t i = 0; i < ptr->len; i++) {
        memc_bounds_check(ptr, i);
        p[i] = (u8) i+2;
    }
}

int main() {
    memc *m = memc_alloc(10);

    fill(m);

    for (size_t i = 0; i < m->len; i++) {
        printf("%hhu ", ((u8*)(m->ptr))[i]);
    }
    putchar('\n');

    memc_nfree(m);
}
