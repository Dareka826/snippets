#ifndef __ARENA_ALLOC_H_mv4ndFREGmFcCdiL
#define __ARENA_ALLOC_H_mv4ndFREGmFcCdiL

#include <stdlib.h>

typedef  struct Arena  Arena;

// Singly-linked list of memory buffers
struct ArenaBuf {
    struct ArenaBuf *next;
    void *ptr;

    #ifdef ARENA_EXTRA
    size_t size;
    #endif
};

// Arena allocator
struct Arena {
    struct ArenaBuf *head;
};


// Init / Destroy
Arena* create_arena();

void free_arena(Arena *ap);
#define nfree_arena(ptr) do { free_arena(ptr); ptr = NULL; } while(0);


// Allocate
void* arena_alloc(Arena *ap, size_t size);
void* arena_alloc0(Arena *ap, size_t size);

void* arena_xalloc(Arena *ap, size_t size);
void* arena_xalloc0(Arena *ap, size_t size);


#ifdef ARENA_EXTRA
// Frees a single buffer from the list (for rare cases)
void arena_mid_free(Arena *ap, void *ptr);
#define arena_mid_nfree(ap, ptr) do { arena_mid_free(ap, ptr); ptr = NULL; } while(0);

size_t arena_get_size(Arena *ap);
size_t arena_get_buffer_size(Arena *ap, void *ptr);
#endif

#endif
