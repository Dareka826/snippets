#include "arena.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#ifdef ARENA_MEM_DBG
// Memory allocation / free debug log {{{
#define C_RESET  "\033[0m"
#define C_RED    "\033[32m"
#define C_GREEN  "\033[32m"
#define C_YELLOW "\033[33m"
#define C_BLUE   "\033[34m"

#define C_MEM   C_GREEN
#define C_ALLOC C_YELLOW
#define C_FREE  C_BLUE

#define _arena_mem_dbg_alloc(type, ptr, reason) do {\
    fprintf(stderr, C_MEM"[MEM]"C_ALLOC" %s:"C_RESET" %p (%s)\n", type, ptr, reason); \
} while(0)

#define _arena_mem_dbg_free(ptr, reason) do {\
    fprintf(stderr, C_MEM"[MEM]"C_FREE" free:"C_RESET" %p (%s)\n", ptr, reason);\
} while(0)

#else
#define _arena_mem_dbg_alloc(type, ptr, rsn) do {} while (0)
#define _arena_mem_dbg_free(ptr, rsn) do {} while (0)
// }}}
#endif

// Init / Deinit {{{
Arena* create_arena() { /*{{{*/
    Arena * const ap = malloc(sizeof(Arena));
    memset(ap, 0, sizeof(Arena));

    _arena_mem_dbg_alloc("malloc", ap, "arena init");

    return ap;
} /*}}}*/

void free_arena(Arena * const ap) { /*{{{*/
    struct ArenaBuf
        *head = ap->head,
        *next = NULL;

    while (head != NULL) {
        next = head->next;

        _arena_mem_dbg_free(head->ptr, "buffer data free");
        free(head->ptr);

        _arena_mem_dbg_free(head, "buffer free");
        free(head);

        head = next;
    }

    _arena_mem_dbg_free(ap, "arena free");
    free(ap);
} /*}}}*/
// }}}

// Allocate {{{
void* arena_alloc(Arena * const ap, size_t size) { /*{{{*/
    struct ArenaBuf * const abp = malloc(sizeof(struct ArenaBuf));
    memset(abp, 0, sizeof(struct ArenaBuf));

    _arena_mem_dbg_alloc("malloc", abp, "buffer init");

    abp->ptr = malloc(size);

    _arena_mem_dbg_alloc("malloc", abp->ptr, "buffer data init");

    #ifdef ARENA_EXTRA
    if (abp->ptr != NULL)
        abp->size = size;
    #endif

    abp->next = ap->head;
    ap->head = abp;

    return abp->ptr;
} /*}}}*/

void* arena_alloc0(Arena * const ap, size_t size) { /*{{{*/
    struct ArenaBuf * const abp = malloc(sizeof(struct ArenaBuf));
    memset(abp, 0, sizeof(struct ArenaBuf));

    _arena_mem_dbg_alloc("malloc", abp, "buffer init");

    abp->ptr = calloc(1, size);

    _arena_mem_dbg_alloc("calloc", abp->ptr, "buffer data init");

    #ifdef ARENA_EXTRA
    if (abp->ptr != NULL)
        abp->size = size;
    #endif

    abp->next = ap->head;
    ap->head = abp;

    return abp->ptr;
} /*}}}*/

void* arena_xalloc(Arena * const ap, size_t size) { /*{{{*/
    void * const p = arena_alloc(ap, size);

    if (p == NULL)
        exit(1);

    return p;
} /*}}}*/

void* arena_xalloc0(Arena * const ap, size_t size) { /*{{{*/
    void * const p = arena_alloc0(ap, size);

    if (p == NULL)
        exit(1);

    return p;
} /*}}}*/
// }}}

#ifdef ARENA_EXTRA
// Extra functions {{{
void arena_mid_free(Arena * const ap, void * const ptr) { /*{{{*/
    struct ArenaBuf
        *head = ap->head,
        *prev = NULL;

    while (head != NULL) {
        if (head->ptr == ptr) {
            // Remove from linked list
            if (prev != NULL)
                 prev->next = head->next;
            else ap->head   = head->next;

            // Free
            _arena_mem_dbg_free(head->ptr, "buffer data mid-free");
            free(head->ptr);

            _arena_mem_dbg_free(head, "buffer mid-free");
            free(head);

            break;
        }

        prev = head;
        head = head->next;
    }
} /*}}}*/

size_t arena_get_size(Arena const * const ap) { /*{{{*/
    struct ArenaBuf const *head = ap->head;
    size_t size = 0;

    while (head != NULL) {
        size += head->size;
        head = head->next;
    }

    return size;
} /*}}}*/

size_t arena_get_buffer_size(Arena const * const ap, void const * const ptr) { /*{{{*/
    struct ArenaBuf const *head = ap->head;

    while (head != NULL) {
        if (head->ptr == ptr)
            return head->size;

        head = head->next;
    }

    return 0;
} /*}}}*/
// }}}
#endif
