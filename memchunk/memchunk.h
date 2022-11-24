#ifndef __MEMCHUNK_H_KQY1xnora93Rx7IZ
#define __MEMCHUNK_H_KQY1xnora93Rx7IZ

#include <stdlib.h>

typedef  struct MemChunk  memc;

struct MemChunk {
    void *ptr;
    size_t len;
};

memc* memc_alloc(size_t size);
memc* memc_alloc0(size_t size);

void memc_free(memc * const mp);
#define memc_nfree(ptr) do { memc_free(ptr); ptr = NULL; } while (0)

unsigned char memc_bounds_check(memc const * const mp, size_t i);
void memc_bounds_xcheck(memc const * const mp, size_t i);

#define MP(type, mptr) ((type)(mptr->ptr))

#endif
