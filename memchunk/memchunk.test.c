#include "memchunk.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../short_types.h"


int main() {
    {
        memc *m = memc_alloc(14);
        assert( m != NULL );
        assert( m->ptr != NULL );
        assert( m->len == 14 );

        ((u8*)(m->ptr))[0]  = 0x45;
        ((u8*)(m->ptr))[13] = 0xCD;

        assert( ((u8*)(m->ptr))[0]  == 0x45 );
        assert( ((u8*)(m->ptr))[13] == 0xCD );

        assert( MP(u8*, m)[0]  == 0x45 );
        assert( MP(u8*, m)[13] == 0xCD );

        assert( memc_bounds_check(m, 0)  == 1 );
        assert( memc_bounds_check(m, 13) == 1 );
        assert( memc_bounds_check(m, 14) == 0 );

        memc_bounds_xcheck(m, 0);
        memc_bounds_xcheck(m, 13);

        memc_nfree(m);
    }

    {
        memc *m = memc_alloc0(25);
        assert( m != NULL );
        assert( m->ptr != NULL );
        assert( m->len == 25 );

        assert( MP(u8*, m)[0]  == 0 );
        assert( MP(u8*, m)[12] == 0 );
        assert( MP(u8*, m)[24] == 0 );

        memc_nfree(m);
    }

    return 0;
}
