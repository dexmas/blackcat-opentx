
#ifndef _BIN_ALLOCATOR_H_
#define _BIN_ALLOCATOR_H_

#include "debug.h"

#if defined(USE_BIN_ALLOCATOR)

#define UMM_H_ATTPACKPRE
#define UMM_H_ATTPACKSUF __attribute__((__packed__))

#define UMM_BEST_FIT
//#define UMM_FIRST_FIT

/* Start addresses and the size of the heap */
#define UMM_MALLOC_CFG_HEAP_ADDR (test_umm_heap)
#define UMM_MALLOC_CFG_HEAP_SIZE (0x8000)

extern void  umm_init(void);
extern void* umm_malloc(size_t size);
extern void* umm_calloc(size_t num, size_t size);
extern void* umm_realloc(void* ptr, size_t size);
extern void  umm_free(void* ptr);

#endif

void *bin_l_alloc(void *ud, void *ptr, size_t osize, size_t nsize);

#endif//_BIN_ALLOCATOR_H_
