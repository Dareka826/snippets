#ifndef __ARENA_ALLOC_H_mv4ndFREGmFcCdiL
#define __ARENA_ALLOC_H_mv4ndFREGmFcCdiL

#include <stdlib.h>

typedef  struct Arena  Arena;

// Singly-linked list of memory buffers
struct ArenaBuf {
    struct ArenaBuf *next;
    void *ptr;
};

// Arena allocator
struct Arena {
    struct ArenaBuf *head;
};


// Init / Destroy
Arena* create_arena();

void free_arena(Arena*);
#define nfree_arena(ptr) do { free_arena(ptr); ptr = NULL; } while(0);


// Allocate
void* arena_alloc(Arena*, size_t);
void* arena_alloc0(Arena*, size_t);

void* arena_xalloc(Arena*, size_t);
void* arena_xalloc0(Arena*, size_t);


#ifdef ARENA_EXTRA
// Frees a single buffer from the list (for rare cases)
void arena_mid_free(Arena*, void*);
#define arena_mid_nfree(ap,ptr) do { arena_mid_free(ap, ptr); ptr = NULL; } while(0);
#endif

#endif
