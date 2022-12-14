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

    #if defined(ARENA_MEM_DBG) && defined(ARENA_DBG_NAME)
    char const * name;
    #endif
};


// Init / Destroy
Arena* create_arena();
#if defined(ARENA_MEM_DBG) && defined(ARENA_DBG_NAME)
Arena* create_arenan(const char * const name);
#else
#define create_arenan(name) create_arena()
#endif

void free_arena(Arena * const ap);
#define nfree_arena(ptr) do { free_arena(ptr); ptr = NULL; } while (0)


// Allocate
void* arena_alloc(Arena * const ap, size_t size);
void* arena_alloc0(Arena * const ap, size_t size);

void* arena_xalloc(Arena * const ap, size_t size);
void* arena_xalloc0(Arena * const ap, size_t size);


#ifdef ARENA_EXTRA
void arena_pop(Arena * const ap);
#define arena_npop(ptr) do { arena_pop(ptr); ptr = NULL; } while (0)

size_t arena_get_size(Arena const * const ap);
size_t arena_get_buffer_size(Arena const * const ap, void const * const ptr);

#ifdef ARENA_UNSAFE
// Frees a single buffer from the list (for rare cases)
void arena_mid_free(Arena * const ap, void * const ptr);
#define arena_mid_nfree(ap, ptr) do { arena_mid_free(ap, ptr); ptr = NULL; } while (0)
#endif
#endif

#endif
