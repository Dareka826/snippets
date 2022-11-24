#include "arena.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../short_types.h"


int main() {
    // Init / Free {{{
    {
        Arena *a = create_arena();
        assert( a != NULL );

        assert( a->head == NULL );
        #if defined(ARENA_MEM_DBG) && defined(ARENA_DBG_NAME)
        assert( a->name == NULL );
        #endif

        nfree_arena(a);
        assert( a == NULL );
    } /*}}}*/

    #if defined(ARENA_MEM_DBG) && defined(ARENA_DBG_NAME)
    // Name (optional) {{{
    {
        Arena *a = create_arenan("test##name");

        assert( a->name != NULL );
        assert( strcmp("test##name", a->name) == 0 );

        free_arena(a);
    } /*}}}*/
    #endif

    // Allocation {{{
    {
        Arena *a = create_arena();

        u8 *p1 = arena_alloc(a, 13);
        assert( p1 != NULL );
        p1[0]  = 0xFC;
        assert( p1[0]  == 0xFC );
        p1[12] = 0x24;
        assert( p1[12] == 0x24 );

        u8 *p2 = arena_alloc0(a, 15);
        assert( p2 != NULL );
        assert( p2[0]  == 0 );
        assert( p2[7]  == 0 );
        assert( p2[14] == 0 );

        u8 *p3 = arena_xalloc(a, 8);
        assert( p3 != NULL );
        p3[0]  = 0xBB;
        assert( p3[0]  == 0xBB );
        p3[7] = 0x6F;
        assert( p3[7] == 0x6F );

        u8 *p4 = arena_xalloc0(a, 27);
        assert( p4 != NULL );
        assert( p4[0]  == 0 );
        assert( p4[13] == 0 );
        assert( p4[26] == 0 );

        // Structure
        assert( a->head->ptr                    == p4 );
        assert( a->head->next->ptr              == p3 );
        assert( a->head->next->next->ptr        == p2 );
        assert( a->head->next->next->next->ptr  == p1 );
        assert( a->head->next->next->next->next == NULL );

        nfree_arena(a);
    } /*}}}*/

    #ifdef ARENA_EXTRA
    // Extra functionality (optional) {{{
    {
        Arena *a = create_arena();

        u8 *p1 = arena_alloc(a, 5);
        u8 *p2 = arena_alloc(a, 7);
        u8 *p3 = arena_alloc(a, 3);
        u8 *p4 = arena_alloc(a, 1);

        // Size {{{
        assert( arena_get_size(a) == 5+7+3+1 );

        assert( a->head->size                   == 1  );
        assert( a->head->next->size             == 3  );
        assert( a->head->next->next->size       == 7  );
        assert( a->head->next->next->next->size == 5  );

        assert( arena_get_buffer_size(a, p4) == 1 );
        assert( arena_get_buffer_size(a, p3) == 3 );
        assert( arena_get_buffer_size(a, p2) == 7 );
        assert( arena_get_buffer_size(a, p1) == 5 );
        // }}}

        // Pop
        arena_pop(a);

        assert( a->head->ptr              == p3 );
        assert( a->head->next->ptr        == p2 );
        assert( a->head->next->next->ptr  == p1 );
        assert( a->head->next->next->next == NULL );

        #ifdef ARENA_UNSAFE
        // Mid-free {{{
        arena_mid_nfree(a, p2);
        assert( p2 == NULL );

        assert( a->head->ptr        == p3 );
        assert( a->head->next->ptr  == p1 );
        assert( a->head->next->next == NULL );
        // }}}

        // Size after mid-free
        assert( arena_get_size(a) == 5+3 );
        #endif

        nfree_arena(a);
    } /*}}}*/
    #endif

    return 0;
}
