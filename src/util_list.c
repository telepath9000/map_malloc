#include "../include/map_malloc.h"

static void init_mem(void)
{
    g_mem = mmap(0, GLOBAL,
            PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    if (g_mem == MAP_FAILED) {
        g_mem = NULL;
        return;
    }
    g_mem->small = NULL;
    g_mem->med = NULL;
    g_mem->large = NULL;
    g_mem->ssize = 0;
    g_mem->msize = 0;
    g_mem->lsize = 0;
    g_mem->total_mem = 0;
}

t_unit *get_list_of_type(t_mem_type type)
{
    if (!g_mem)
        return NULL;
    switch (type)
    {
    case small:
        return g_mem->small;
    case med:
        return g_mem->med;
    case large:
        return g_mem->large;
    }
    return NULL;
}

t_unit  **get_modifiable_list_of_type(t_mem_type type)
{
    if (!g_mem)
        return NULL;
    switch (type)
    {
    case small:
        return &g_mem->small;
    case med:
        return &g_mem->med;
    case large:
        return &g_mem->large;
    }
    return NULL;
}

/*static size_t   get_alloc_size_by_type(t_mem_type type)
{
    switch (type)
    {
    case small:
        return SMALL_ALLOC;
    case med:
        return MED_ALLOC;
    case large:
        return LARGE_ALLOC;
    }
    return 0;
}*/

void            *init_chunk(t_unit *mem, size_t size)
{
    void    *ret;
    t_mem_type type;

    type = get_type(size);
    if (!g_mem)
        init_mem();
    if (!g_mem)
        return NULL;
    
    // Initialize the chunk metadata
    mem->prev = NULL;
    mem->next = NULL;
    mem->filled = 1;  // We're allocating the first slot
    memset(mem->table, 0, sizeof(mem->table));
    mem->table[0] = size;  // Mark first slot as allocated with the size
    
    // Return the address of the first allocation
    ret = get_address(mem, 0, type);
    return ret;
}