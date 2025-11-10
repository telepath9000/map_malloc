#include "../include/map_malloc.h"

static void add_chunk(t_unit *chunk, size_t type)
{
    t_unit  **to_mod;

    to_mod = get_modifiable_list_of_type(type);  // Fixed: use type directly, not get_type(type)
    chunk->next = *to_mod;
    if (*to_mod)
        (*to_mod)->prev = chunk;
    *to_mod = chunk;  // Fixed: assign chunk, not chunk->next
}

void        *alloc_core(size_t size)
{
    void    *ret;
    void    *chunk;
    size_t  type;
    size_t  total;

    type = get_type(size);
    total = get_alloc_size(size);
    chunk = mmap(0, total, PROT_READ | PROT_WRITE,
            MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    if (chunk == MAP_FAILED)
        return NULL;
    ret = init_chunk(chunk, size);
    add_to_size(type, size);
    add_chunk(chunk, type);  // Fixed: pass type enum, not size
    return ret;
}