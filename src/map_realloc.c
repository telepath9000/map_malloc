#include "../include/map_malloc.h"

void        *map_realloc(void *ptr, size_t size)
{
    void    *ret;

    ret = NULL;
    if (!ptr)
        return map_malloc(size);
    for (int i = 0; i < 3; i++)
    {
        ret = free_or_realloc_type(ptr, g_type_list[i], 1, size);
        if (ret)
            return ret;
    }
    return ret;
}
