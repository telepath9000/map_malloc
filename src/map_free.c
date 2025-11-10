#include "../include/map_malloc.h"

static void reduce_size(t_mem_type type)
{
    switch (type)
    {
    case small:
        g_mem->ssize--;
        break ;
    case med:
        g_mem->msize--;
        break ;
    case large:
        g_mem->lsize--;
    }
}

void free_core(t_unit *target, t_mem_type type, size_t size)
{
    t_unit **list_of_type_head;

    list_of_type_head = get_modifiable_list_of_type(type);
    if (!list_of_type_head)
        return;
    reduce_size(type);
    if (target->prev)
        target->prev->next = target->next;
    else
        *list_of_type_head = target->next;
    if (target->next)
        target->next->prev = target->prev;
    error_handle_munmap(target, size);
}

static void *initialize_and_free_partition(void *ptr, t_unit *chunk, t_mem_type type, int is_realloc, size_t size, size_t end)
{
    void    *ret;
    size_t  i;
    size_t  chunk_size;

    ret = NULL;
    i = 0;
    while (i < end)
    {
        if (chunk->table[i] && ptr == (void *)get_address(chunk, i, type))
        {
            // Calculate chunk allocation size before we modify anything
            if (type == large)
                chunk_size = get_alloc_size(chunk->table[i]);
            else if (type == small)
                chunk_size = get_alloc_size(SMALL_BYTES);
            else
                chunk_size = get_alloc_size(MED_BYTES);

            if (is_realloc)
            {
                ret = map_malloc(size);
                if (ret)
                    malcpy(ret, ptr, chunk->table[i], size);
                else
                    return NULL;  // Allocation failed, don't free old memory
            }
            else
                ret = ptr;
            
            chunk->table[i] = 0;
            chunk->filled--;
            
            if (!chunk->filled)
            {
                free_core(chunk, type, chunk_size);
            }
            return ret;
        }
        i++;
    }
    return ret;
}

void *free_or_realloc_type(void *ptr, t_mem_type type, int is_realloc, size_t size)
{
    t_unit  *cur;
    t_unit  *next;
    void    *ret;
    size_t  end;

    end = type == large ? 1 : 100;
    cur = get_list_of_type(type);
    ret = NULL;
    while (cur)
    {
        next = cur->next;  // Save next pointer before potential free
        ret = initialize_and_free_partition(ptr, cur, type, is_realloc, size, end);
        if (ret)
            return ret;
        cur = next;  // Use saved next pointer
    }
    return NULL;
}

void map_free(void *ptr)
{
    if (ptr && g_mem)
    {
        for (int i = 0; i < 3; i++)
            if (free_or_realloc_type(ptr, g_type_list[i], 0, 0))
                break;
        if (!(g_mem->ssize + g_mem->msize + g_mem->lsize))
        {
            error_handle_munmap(g_mem, GLOBAL);
            g_mem = NULL;
        }
    }
}