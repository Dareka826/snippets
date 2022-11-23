#include "memchunk.h"
#include <stdlib.h>
#include <string.h>

memc* memc_alloc(size_t size) {
    memc * const mp = malloc(sizeof(memc));
    memset(mp, 0, sizeof(memc));

    mp->ptr = malloc(size);
    if (mp->ptr != NULL)
        mp->len = size;

    return mp;
}

memc* memc_alloc0(size_t size) {
    memc * const mp = malloc(sizeof(memc));
    memset(mp, 0, sizeof(memc));

    mp->ptr = calloc(1, size);
    if (mp->ptr != NULL)
        mp->len = size;

    return mp;
}

void memc_free(memc * const mp) {
    free(mp->ptr);
    free(mp);
}

void memc_bounds_check(memc const * const mp, size_t i) {
    if(i < 0 || i >= mp->len) {
        fprintf(stderr, "Bounds check failed! [%lu / %lu]\n", i, mp->len);
        exit(1);
    }
}
