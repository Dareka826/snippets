#include <stdio.h>
#include <stdlib.h>
#include "../short_types.h"
#include "memchunk.h"

void fill(memc * const p) {

    for (size_t i = 0; i < p->len; i++) {
        memc_bounds_check(p, i);
        MP(u8*, p)[i] = (u8) i+2;
    }
}

int main() {
    memc *m = memc_alloc(10);

    fill(m);

    for (size_t i = 0; i < m->len; i++) {
        printf("%hhu ", MP(u8*, m)[i]);
    }
    putchar('\n');

    memc_nfree(m);
}
