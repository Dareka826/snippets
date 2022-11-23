#include "arena.h"
#include <stdlib.h>
#include <stdio.h>

#ifdef ARENA_MEM_DBG
#define C_RESET  "\033[0m"
#define C_GREEN  "\033[32m"
#define C_YELLOW "\033[33m"
#define C_BLUE   "\033[34m"
#endif

// Init / Deinit {{{
Arena* create_arena() { /*{{{*/
    Arena *ap = malloc(sizeof(Arena));
    memset(ap, 0, sizeof(Arena));

    #ifdef ARENA_MEM_DBG
    fprintf(stderr, C_GREEN"[MEM]"C_YELLOW" malloc:"C_RESET" %p\n", ap);
    #endif

    return ap;
} /*}}}*/

void free_arena(Arena *ap) { /*{{{*/
    struct ArenaBuf
        *head = ap->head,
        *next = NULL;

    while (head != NULL) {
        next = head->next;

        #ifdef ARENA_MEM_DBG
        fprintf(stderr, C_GREEN"[MEM]"C_BLUE" free:"C_RESET" %p\n", head->ptr);
        #endif
        free(head->ptr);

        #ifdef ARENA_MEM_DBG
        fprintf(stderr, C_GREEN"[MEM]"C_BLUE" free:"C_RESET" %p\n", head);
        #endif
        free(head);

        head = next;
    }

    #ifdef ARENA_MEM_DBG
    fprintf(stderr, C_GREEN"[MEM]"C_BLUE" free:"C_RESET" %p\n", ap);
    #endif
    free(ap);
} /*}}}*/
// }}}

// Allocate {{{
void* arena_alloc(Arena *ap, size_t size) { /*{{{*/
    struct ArenaBuf *abp = malloc(sizeof(struct ArenaBuf));
    memset(abp, 0, sizeof(struct ArenaBuf));

    #ifdef ARENA_MEM_DBG
    fprintf(stderr, C_GREEN"[MEM]"C_YELLOW" malloc:"C_RESET" %p\n", abp);
    #endif

    abp->ptr = malloc(size);
    if (abp->ptr != NULL)
        abp->size = size;

    #ifdef ARENA_MEM_DBG
    fprintf(stderr, C_GREEN"[MEM]"C_YELLOW" malloc:"C_RESET" %p\n", abp->ptr);
    #endif

    abp->next = ap->head;
    ap->head = abp;

    return abp->ptr;
} /*}}}*/

void* arena_alloc0(Arena *ap, size_t size) { /*{{{*/
    struct ArenaBuf *abp = malloc(sizeof(struct ArenaBuf));
    memset(abp, 0, sizeof(struct ArenaBuf));

    #ifdef ARENA_MEM_DBG
    fprintf(stderr, C_GREEN"[MEM]"C_YELLOW" malloc:"C_RESET" %p\n", abp);
    #endif

    abp->ptr = calloc(1, size);
    if (abp->ptr != NULL)
        abp->size = size;

    #ifdef ARENA_MEM_DBG
    fprintf(stderr, C_GREEN"[MEM]"C_YELLOW" calloc:"C_RESET" %p\n", abp->ptr);
    #endif

    abp->next = ap->head;
    ap->head = abp;

    return abp->ptr;
} /*}}}*/

void* arena_xalloc(Arena *ap, size_t size) { /*{{{*/
    void *p = arena_alloc(ap, size);

    if (p == NULL)
        exit(1);

    return p;
} /*}}}*/

void* arena_xalloc0(Arena *ap, size_t size) { /*{{{*/
    void *p = arena_alloc0(ap, size);

    if (p == NULL)
        exit(1);

    return p;
} /*}}}*/
// }}}

#ifdef ARENA_EXTRA
// Extra functions {{{
void arena_mid_free(Arena *ap, void *ptr) { /*{{{*/
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
            #ifdef ARENA_MEM_DBG
            fprintf(stderr, C_GREEN"[MEM]"C_BLUE" free:"C_RESET" %p\n", head->ptr);
            #endif
            free(head->ptr);

            #ifdef ARENA_MEM_DBG
            fprintf(stderr, C_GREEN"[MEM]"C_BLUE" free:"C_RESET" %p\n", head);
            #endif
            free(head);

            break;
        }

        prev = head;
        head = head->next;
    }
} /*}}}*/

size_t arena_get_size(Arena *ap) {
    struct ArenaBuf *head = ap->head;
    size_t size = 0;

    while (head != NULL) {
        size += head->size;
        head = head->next;
    }

    return size;
}

size_t arena_get_buffer_size(Arena *ap, void *ptr) {
    struct ArenaBuf *head = ap->head;

    while (head != NULL) {
        if (head->ptr == ptr)
            return head->size;

        head = head->next;
    }

    return 0;
}
// }}}
#endif
