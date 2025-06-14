#include "../include/map_malloc.h"

static void *check_large(t_unit *cur, void *ptr, size_t size)
{
    void    *ret;

    ret = NULL;
    if (ptr == (void *)get_address(cur, 0, large) &&
        get_alloc_size(cur->unit.large->size) != get_alloc_size(size))
    {
        ret = map_malloc(size);
        malcpy(ret, ptr, cur->unit.large->size, size);
        free_core((void *)cur, cur->unit.large->size);
    }
    return (ret);
}

static void *check_med(t_unit *cur, void *ptr, size_t size)
{
    void    *ret;
    int		i;

    ret = NULL;
    i = -1;
    while (++i < 100)
    {
        if (cur->unit.med->table[i] && ptr == (void *)get_address(cur, i, med))
        {
            if (size > MED_BYTES || size < SMALL_BYTES)
            {
                cur->unit.med->table[i] = 0;
                ret = map_malloc(size);
                malcpy(ret, ptr, cur->unit.med->table[i], size);
                if (!--(cur->unit.med->filled))
                    free_core((void *)cur, MED_BYTES);
                return (ret);
            }
            else
                cur->unit.med->table[i] = size;
            break ;
        }
    }
    return (ret);
}

static void *check_small(t_unit *cur, void *ptr, size_t size)
{
    void    *ret;
    int     i;

    ret = NULL;
    i = -1;
    while (++i < 100)
    {
        if (cur->unit.small->table[i] &&
            ptr == (void *)get_address(cur, i, small))
        {
            if (size > SMALL_BYTES)
            {
                cur->unit.small->table[i] = 0;
                ret = map_malloc(size);
                malcpy(ret, ptr, cur->unit.small->table[i], size);
                if (!(--cur->unit.small->filled))
                    free_core(cur, SMALL_BYTES);
                return (ret);
            }
            else
                cur->unit.small->table[i] = size;
            break ;
        }
    }
    return (ret);
}

static void *iter_list(void *ptr, t_mem_type type, t_unit *list, size_t size)
{
    t_unit  *cur;
    void    *ret;

    cur = list;
    ret = NULL;
    while (cur)
    {
        if (type == small)
            ret = check_small(list, ptr, size);
        else if (type == med)
            ret = check_med(list, ptr, size);
        else
            ret = check_large(list, ptr, size);
        if (ret)
            break ;
        if (cur)
            cur = cur->next;
    }
    return ret;
}

void        *map_realloc(void *ptr, size_t size)
{
    void	    *ret;

    if (!ptr)
        return map_malloc(size);
    if (!(ret = iter_list(ptr, small, g_mem->small, size)) &&
            !(ret = iter_list(ptr, med, g_mem->med, size)) &&
            !(ret = iter_list(ptr, large, g_mem->large, size)))
    {
        map_free(ptr);
        return NULL;
    }
    return (ret);
}
