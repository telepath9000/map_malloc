#ifndef _GNU_SOURCE
# define _GNU_SOURCE
#endif

#ifndef MAP_MALLOC_H
# define MAP_MALLOC_H

# include <sys/mman.h>
# include <unistd.h>
# include <sys/resource.h>
# include <limits.h>
# include <sys/time.h>
# include <string.h>
# include <fcntl.h>

# define SMALL_BYTES 1024
# define MED_BYTES 4096
# define SIZES sizeof(t_unit *) * 3
# define GLOBAL SIZES + (sizeof(size_t) * 3) + sizeof(rlim_t)
# define LARGE 1
# define UNIT_ALLOC (sizeof(t_unit *) * 2)
# define SMALL_ALLOC UNIT_ALLOC + (sizeof(int) * 101)
# define MED_ALLOC UNIT_ALLOC + (sizeof(int) * 101)
# define LARGE_ALLOC UNIT_ALLOC + sizeof(size_t)

typedef	enum		e_mem_type
{
	small,
	med,
	large
}					t_mem_type;

typedef struct		s_unit
{
	struct s_unit	*next;
	struct s_unit	*prev;
	int				filled;
	size_t			table[100];
}					t_unit;

typedef struct		s_mem
{
	t_unit			*small;
	t_unit			*med;
	t_unit			*large;
	size_t			ssize;
	size_t			msize;
	size_t			lsize;
	rlim_t			total_mem;
}					t_mem;

extern t_mem		*g_mem;

extern t_mem_type	g_type_list[3];	

void				map_free(void *ptr);
void 				free_core(t_unit *target, t_mem_type type, size_t size);
void				*free_or_realloc_type(void *ptr, t_mem_type type, int is_realloc, size_t size);

void				*map_malloc(size_t size);

void				*map_realloc(void *ptr, size_t size);

void				show_alloc_mem(void);

void				*alloc_core(size_t size);

/*
** utilities
*/

size_t				get_alloc_size(size_t size);
int					check_limit(size_t size);
int					error_handle_munmap(void *target, size_t size);
void				set_limit(size_t type, size_t inc);
void				malcpy(void *dest, void *src, size_t ref_len, size_t len);
t_unit				*get_list_of_type(t_mem_type type);
t_unit				**get_modifiable_list_of_type(t_mem_type type);

size_t				get_type(size_t size);
void				*init_chunk(t_unit *mem, size_t size);
void				set_tail(t_unit *chunk, size_t type);
t_unit				*find_slot(t_unit *chunk, size_t type);

void				add_to_size(size_t type, size_t size);

void				*get_address(void *cur, size_t i, t_mem_type type);

void				log_output();

#endif
