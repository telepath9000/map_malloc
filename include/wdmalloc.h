#ifndef WD_MALLOC_H
# define WD_MALLOC_H

# include <sys/mman.h>
# include <unistd.h>
# include <sys/resource.h>
# include <limits.h>

# define TINY	32768	/*32 Kb */
# define SMALL	131072	/*128 Kb*/


typedef struct		s_mem_info
{
	void			*ptr;
	int				type;
	size_t			size;
}					t_mem_info;

void				wd_free(void *ptr);
void				wd_malloc(size_t size);
void*				wd_realloc(void *ptr, size_t size);

#endif
