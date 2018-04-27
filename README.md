# map\_malloc

A malloc implementation that strictly uses mapped virtual memory spaces.

# Structure

Memory spaces are pre-cached in chunks based on size to avoid numerous requests to the kernel. Chunks are stored in three seperate lists, also based on size. Small and medium sized chunks are partitioned into a finite number of small and medium sized allocations as defined by the header. The macros SMALL\_BYTES and MED\_BYTES can be changed to affect this behavior. If there is no available memory in a chunk, a new chunk is appended to the list. If an allocation is freed for use and it is the last to be freed for use, the chunk is removed from the list and unmapped from memory.

## Usage

`void	*map_malloc(size_t size);`

Returns a pointer to a newly reserved memory space of size *size* in bytes.

`void	map_free(void *ptr);`

Removes the address

`void	*map_realloc(void *ptr, size_t size);`

Takes the pointer *ptr* and returns address of new memory space of size *size*. The contents of *ptr* are copied up to *size* in the new memory space.

`void	show_alloc_mem(void);`

Displays all memory that is currently being managed by map\_malloc.

# Install

Run

`make`

in the repo directroy to build shared library file.

Alternatively, to install system-wide run

`sudo make install`

in the repo directory.

Once installed, in order to compile with the library use

`-lmap_malloc$(uname -m)_$(uname -s)`

as a compile flag and include library with

`#include <map_malloc.h>`

in the target source files.
