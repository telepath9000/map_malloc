#include "../include/map_malloc.h"

void    set_limit(size_t type, size_t inc)
{
    if (inc)
    {
        if (type == SMALL_BYTES)
            g_mem->total_mem += get_alloc_size(SMALL_BYTES);
        else if (type == MED_BYTES)
            g_mem->total_mem += get_alloc_size(MED_BYTES);
        else
            g_mem->total_mem += (LARGE_ALLOC + type);
    }
    else
    {
        if (type <= SMALL_BYTES)
            g_mem->total_mem -= get_alloc_size(SMALL_BYTES);
        else if (type <= MED_BYTES && type > SMALL_BYTES)
            g_mem->total_mem -= get_alloc_size(MED_BYTES);
        else
            g_mem->total_mem -= (LARGE_ALLOC + type);
    }
}

int     error_handle_munmap(void *target, size_t size)
{
    int err;
    int is_global;

    is_global = (g_mem && (g_mem->ssize + g_mem->msize + g_mem->lsize)) ? 1 : 0;
    err = munmap(target, size);
    if (err && write(2, "ERROR: Something bad happened.\n", 31) > -1)
        return (-1);
    else if (is_global)
        set_limit(size, 0);
    return (0);
}

int     check_limit(size_t size)
{
    struct rlimit   rlim;
    static rlim_t   limit = 0;
    int             ret;

    ret = 0;
    if (limit == 0)
    {
        ret = getrlimit(RLIMIT_AS, &rlim);
        if (ret == 0)
            limit = rlim.rlim_cur;
    }
    else if (g_mem &&
            (g_mem->total_mem + (rlim_t)get_alloc_size(size)) > limit)
        ret = -1;
    return (ret);
}

size_t  get_alloc_size(size_t size)
{
    size_t ret;
    size_t page;

    ret = 0;
    page = (size_t)getpagesize();
    if (size <= SMALL_BYTES)
        ret = ((SMALL_BYTES * 100) > page) ? 
            ((((SMALL_BYTES * 100) / page) + 1) * page) + SMALL_ALLOC : 
            page + SMALL_ALLOC;
    else if (size > SMALL_BYTES && size <= MED_BYTES)
        ret = ((MED_BYTES * 100) > page) ? 
            ((((MED_BYTES * 100) / page) + 1) * page) + MED_ALLOC : 
            page + MED_ALLOC;
    else if (size > MED_BYTES)
        ret = (size > page) ?
            (((size / page) + 1) * page) + LARGE_ALLOC : 
            page + LARGE_ALLOC + size;
    return (ret);
}

void    malcpy(void *dest, void *src, size_t ref_len, size_t len)
{
    size_t  i;

    i = 0;
    while (i < len && i < ref_len)
    {
        ((char *)dest)[i] = ((char *)src)[i];
        i++;
    }
}

void    add_to_size(size_t type, size_t size)
{
    g_mem->total_mem += get_alloc_size(size);
    if (type == small)
        g_mem->ssize++;
    else if (type == med)
        g_mem->msize++;
    else
        g_mem->lsize++;
}

size_t  get_type(size_t size)
{
    if (size <= SMALL_BYTES)
        return small;
    else if (size <= MED_BYTES)
        return med;
    else
        return large;
}

void    *get_address(void *cur, size_t i, t_mem_type type)
{
    // The chunk starts at 'cur', and t_unit structure is at the beginning
    // User data starts after the entire t_unit structure
    if (type == small)
        return (char *)cur + sizeof(t_unit) + (i * SMALL_BYTES);
    else if (type == med)
        return (char *)cur + sizeof(t_unit) + (i * MED_BYTES);
    else if (type == large)
        return (char *)cur + sizeof(t_unit);
    return NULL;
}