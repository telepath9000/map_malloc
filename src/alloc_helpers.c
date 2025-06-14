#include "../include/map_malloc.h"

void    *get_address(void *cur, size_t i, t_mem_type type)
{
    if (type == small)
        return (char *)cur + SMALL_ALLOC + (i * SMALL_BYTES);
    else if (type == med)
        return (char *)cur + MED_ALLOC + (i * MED_BYTES);
    else if (type == large)
        return (char *)cur + LARGE_ALLOC;
    return NULL;
}
