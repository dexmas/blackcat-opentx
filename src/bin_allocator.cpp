#include <stdlib.h>
#include <string.h>
#include "opentx.h"

#include "bin_allocator.h"

#if defined(USE_BIN_ALLOCATOR)

char test_umm_heap[UMM_MALLOC_CFG_HEAP_SIZE];

UMM_H_ATTPACKPRE typedef struct umm_ptr_t {
    uint16_t next;
    uint16_t prev;
} UMM_H_ATTPACKSUF umm_ptr;


UMM_H_ATTPACKPRE typedef struct umm_block_t {
    union {
        umm_ptr used;
    } header;
    union {
        umm_ptr free;
        uint8_t data[4];
    } body;
} UMM_H_ATTPACKSUF umm_block;

#define UMM_FREELIST_MASK (0x8000)
#define UMM_BLOCKNO_MASK  (0x7FFF)

umm_block* umm_heap = NULL;
uint16_t umm_numblocks = 0;

#define UMM_NUMBLOCKS  (umm_numblocks)
#define UMM_BLOCK_LAST (UMM_NUMBLOCKS - 1)

#define UMM_BLOCK(b)  (umm_heap[b])

#define UMM_NBLOCK(b) (UMM_BLOCK(b).header.used.next)
#define UMM_PBLOCK(b) (UMM_BLOCK(b).header.used.prev)
#define UMM_NFREE(b)  (UMM_BLOCK(b).body.free.next)
#define UMM_PFREE(b)  (UMM_BLOCK(b).body.free.prev)
#define UMM_DATA(b)   (UMM_BLOCK(b).body.data)

static uint16_t umm_blocks(size_t size) 
{
    /*
     * The calculation of the block size is not too difficult, but there are
     * a few little things that we need to be mindful of.
     *
     * When a block removed from the free list, the space used by the free
     * pointers is available for data. That's what the first calculation
     * of size is doing.
     */

    if (size <= (sizeof(((umm_block*)0)->body)))
        return(1);

    /*
     * If it's for more than that, then we need to figure out the number of
     * additional whole blocks the size of an umm_block are required.
     */

    size -= (1 + (sizeof(((umm_block*)0)->body)));

    return(2 + size / (sizeof(umm_block)));
}

/* ------------------------------------------------------------------------
 * Split the block `c` into two blocks: `c` and `c + blocks`.
 *
 * - `new_freemask` should be `0` if `c + blocks` used, or `UMM_FREELIST_MASK`
 *   otherwise.
 *
 * Note that free pointers are NOT modified by this function.
 */
static void umm_split_block(uint16_t c, uint16_t blocks, uint16_t new_freemask) 
{
    UMM_NBLOCK(c + blocks) = (UMM_NBLOCK(c) & UMM_BLOCKNO_MASK) | new_freemask;
    UMM_PBLOCK(c + blocks) = c;

    UMM_PBLOCK(UMM_NBLOCK(c) & UMM_BLOCKNO_MASK) = (c + blocks);
    UMM_NBLOCK(c) = (c + blocks);
}

static void umm_disconnect_from_free_list(uint16_t c) 
{
    /* Disconnect this block from the FREE list */
    UMM_NFREE(UMM_PFREE(c)) = UMM_NFREE(c);
    UMM_PFREE(UMM_NFREE(c)) = UMM_PFREE(c);

    /* And clear the free block indicator */
    UMM_NBLOCK(c) &= (~UMM_FREELIST_MASK);
}

/* ------------------------------------------------------------------------
 * The umm_assimilate_up() function does not assume that UMM_NBLOCK(c)
 * has the UMM_FREELIST_MASK bit set. It only assimilates up if the
 * next block is free.
 */
static void umm_assimilate_up(uint16_t c) 
{
    if (UMM_NBLOCK(UMM_NBLOCK(c)) & UMM_FREELIST_MASK) 
    {
        /*
         * The next block is a free block, so assimilate up and remove it from
         * the free list
         */

        /* Disconnect the next block from the FREE list */

        umm_disconnect_from_free_list(UMM_NBLOCK(c));

        /* Assimilate the next block with this one */

        UMM_PBLOCK(UMM_NBLOCK(UMM_NBLOCK(c)) & UMM_BLOCKNO_MASK) = c;
        UMM_NBLOCK(c) = UMM_NBLOCK(UMM_NBLOCK(c)) & UMM_BLOCKNO_MASK;
    }
}

/* ------------------------------------------------------------------------
 * The umm_assimilate_down() function assumes that UMM_NBLOCK(c) does NOT
 * have the UMM_FREELIST_MASK bit set. In other words, try to assimilate
 * up before assimilating down.
 */
static uint16_t umm_assimilate_down(uint16_t c, uint16_t freemask) 
{
    // We are going to assimilate down to the previous block because
    UMM_NBLOCK(UMM_PBLOCK(c)) = UMM_NBLOCK(c) | freemask;
    UMM_PBLOCK(UMM_NBLOCK(c)) = UMM_PBLOCK(c);

    return(UMM_PBLOCK(c));
}

void umm_init(void) 
{
    /* init heap pointer and size, and memset it to 0 */
    umm_heap = (umm_block*)UMM_MALLOC_CFG_HEAP_ADDR;
    umm_numblocks = (UMM_MALLOC_CFG_HEAP_SIZE / sizeof(umm_block));
    memset(umm_heap, 0x00, UMM_MALLOC_CFG_HEAP_SIZE);

    /* Set up umm_block[0], which just points to umm_block[1] */
    UMM_NBLOCK(0) = 1;
    UMM_NFREE(0)  = 1;
    UMM_PFREE(0)  = 1;

    /*
     * Now, we need to set the whole heap space as a huge free block. We should
     * not touch umm_block[0], since it's special: umm_block[0] is the head of
     * the free block list. It's a part of the heap invariant.
     *
     * See the detailed explanation at the beginning of the file.
     *
     * umm_block[1] has pointers:
     *
     * - next `umm_block`: the last one umm_block[n]
     * - prev `umm_block`: umm_block[0]
     *
     * Plus, it's a free `umm_block`, so we need to apply `UMM_FREELIST_MASK`
     *
     * And it's the last free block, so the next free block is 0 which marks
     * the end of the list. The previous block and free block pointer are 0
     * too, there is no need to initialize these values due to the init code
     * that memsets the entire umm_ space to 0.
     */
    UMM_NBLOCK(1) = UMM_BLOCK_LAST | UMM_FREELIST_MASK;

    /*
     * Last umm_block[n] has the next block index at 0, meaning it's
     * the end of the list, and the previous block is umm_block[1].
     *
     * The last block is a special block and can never be part of the
     * free list, so its pointers are left at 0 too.
     */

    UMM_PBLOCK(UMM_BLOCK_LAST) = 1;
}

/* ------------------------------------------------------------------------
 * Must be called only from within critical sections guarded by
 * UMM_CRITICAL_ENTRY() and UMM_CRITICAL_EXIT().
 */
static void umm_free_core(void* ptr) 
{
    uint16_t c;

    /*
     * FIXME: At some point it might be a good idea to add a check to make sure
     *        that the pointer we're being asked to free up is actually within
     *        the umm_heap!
     *
     * NOTE:  See the new umm_info() function that you can use to see if a ptr is
     *        on the free list!
     */

     /* Figure out which block we're in. Note the use of truncated division... */

    c = (((uint32_t*)ptr) - (uint32_t*)(&(umm_heap[0]))) / sizeof(umm_block);

    /* Now let's assimilate this block with the next one if possible. */

    umm_assimilate_up(c);

    /* Then assimilate with the previous block if possible */

    if (UMM_NBLOCK(UMM_PBLOCK(c)) & UMM_FREELIST_MASK) 
    {
        c = umm_assimilate_down(c, UMM_FREELIST_MASK);
    }
    else 
    {
        /*
         * The previous block is not a free block, so add this one to the head
         * of the free list
         */
        UMM_PFREE(UMM_NFREE(0)) = c;
        UMM_NFREE(c) = UMM_NFREE(0);
        UMM_PFREE(c) = 0;
        UMM_NFREE(0) = c;

        UMM_NBLOCK(c) |= UMM_FREELIST_MASK;
    }
}

void umm_free(void* ptr) 
{
    if (umm_heap == NULL) 
        umm_init();

    /* If we're being asked to free a NULL pointer, well that's just silly! */
    if ((void*)0 == ptr) 
        return;

    /* Free the memory withing a protected critical section */
    umm_free_core(ptr);
}

/* ------------------------------------------------------------------------
 * Must be called only from within critical sections guarded by
 * UMM_CRITICAL_ENTRY() and UMM_CRITICAL_EXIT().
 */
static void* umm_malloc_core(size_t size) 
{
    uint16_t blocks;
    uint16_t blockSize = 0;

    uint16_t bestSize;
    uint16_t bestBlock;

    uint16_t cf;

    blocks = umm_blocks(size);

    /*
     * Now we can scan through the free list until we find a space that's big
     * enough to hold the number of blocks we need.
     *
     * This part may be customized to be a best-fit, worst-fit, or first-fit
     * algorithm
     */

    cf = UMM_NFREE(0);

    bestBlock = UMM_NFREE(0);
    bestSize = 0x7FFF;

    while (cf) 
    {
        blockSize = (UMM_NBLOCK(cf) & UMM_BLOCKNO_MASK) - cf;

#if defined UMM_BEST_FIT
        if ((blockSize >= blocks) && (blockSize < bestSize)) {
            bestBlock = cf;
            bestSize = blockSize;
        }
#elif defined UMM_FIRST_FIT
        /* This is the first block that fits! */
        if ((blockSize >= blocks))
            break;
#else
        #error "UMM FEET type not configured"
#endif

        cf = UMM_NFREE(cf);
    }

    if (0x7FFF != bestSize) 
    {
        cf = bestBlock;
        blockSize = bestSize;
    }

    if (UMM_NBLOCK(cf) & UMM_BLOCKNO_MASK && blockSize >= blocks) 
    {
        /*
         * This is an existing block in the memory heap, we just need to split off
         * what we need, unlink it from the free list and mark it as in use, and
         * link the rest of the block back into the freelist as if it was a new
         * block on the free list...
         */

        if (blockSize == blocks) 
        {
            /* It's an exact fit and we don't neet to split off a block. */
            /* Disconnect this block from the FREE list */
            umm_disconnect_from_free_list(cf);
        }
        else 
        {
            /* It's not an exact fit and we need to split off a block. */
            /*
             * split current free block `cf` into two blocks. The first one will be
             * returned to user, so it's not free, and the second one will be free.
             */
            umm_split_block(cf, blocks, UMM_FREELIST_MASK /*new block is free*/);

            /*
             * `umm_split_block()` does not update the free pointers (it affects
             * only free flags), but effectively we've just moved beginning of the
             * free block from `cf` to `cf + blocks`. So we have to adjust pointers
             * to and from adjacent free blocks.
             */

             /* previous free block */
            UMM_NFREE(UMM_PFREE(cf)) = cf + blocks;
            UMM_PFREE(cf + blocks) = UMM_PFREE(cf);

            /* next free block */
            UMM_PFREE(UMM_NFREE(cf)) = cf + blocks;
            UMM_NFREE(cf + blocks) = UMM_NFREE(cf);
        }

    }
    else 
    {
        /* Out of memory */
        return((void*)NULL);
    }

    return((void*)&UMM_DATA(cf));
}

void* umm_malloc(size_t size) 
{
    void* ptr = NULL;

    if (umm_heap == NULL) 
        umm_init();

    /*
     * the very first thing we do is figure out if we're being asked to allocate
     * a size of 0 - and if we are we'll simply return a null pointer. if not
     * then reduce the size by 1 byte so that the subsequent calculations on
     * the number of blocks to allocate are easier...
     */
    if (0 == size)
        return(ptr);

    /* Allocate the memory withing a protected critical section */
    ptr = umm_malloc_core(size);

    return(ptr);
}

void* umm_realloc(void* ptr, size_t size) {

    uint16_t blocks;
    uint16_t blockSize;
    uint16_t prevBlockSize = 0;
    uint16_t nextBlockSize = 0;

    uint16_t c;

    size_t curSize;

    if (umm_heap == NULL)
        umm_init();

    /*
     * This code looks after the case of a NULL value for ptr. The ANSI C
     * standard says that if ptr is NULL and size is non-zero, then we've
     * got to work the same a malloc(). If size is also 0, then our version
     * of malloc() returns a NULL pointer, which is OK as far as the ANSI C
     * standard is concerned.
     */
    if (((void*)NULL == ptr))
        return(umm_malloc(size));

    /*
     * Now we're sure that we have a non_NULL ptr, but we're not sure what
     * we should do with it. If the size is 0, then the ANSI C standard says that
     * we should operate the same as free.
     */
    if (0 == size) 
    {
        umm_free(ptr);
        return((void*)NULL);
    }

    /*
     * Otherwise we need to actually do a reallocation. A naiive approach
     * would be to malloc() a new block of the correct size, copy the old data
     * to the new block, and then free the old block.
     *
     * While this will work, we end up doing a lot of possibly unnecessary
     * copying. So first, let's figure out how many blocks we'll need.
     */
    blocks = umm_blocks(size);

    /* Figure out which block we're in. Note the use of truncated division... */

    c = (((uint32_t*)ptr) - (uint32_t*)(&(umm_heap[0]))) / sizeof(umm_block);

    /* Figure out how big this block is ... the free bit is not set :-) */

    blockSize = (UMM_NBLOCK(c) - c);

    /* Figure out how many bytes are in this block */

    curSize = (blockSize * sizeof(umm_block)) - (sizeof(((umm_block*)0)->header));

    /* Now figure out if the previous and/or next blocks are free as well as
     * their sizes - this will help us to minimize special code later when we
     * decide if it's possible to use the adjacent blocks.
     *
     * We set prevBlockSize and nextBlockSize to non-zero values ONLY if they
     * are free!
     */
    if ((UMM_NBLOCK(UMM_NBLOCK(c)) & UMM_FREELIST_MASK)) {
        nextBlockSize = (UMM_NBLOCK(UMM_NBLOCK(c)) & UMM_BLOCKNO_MASK) - UMM_NBLOCK(c);
    }

    if ((UMM_NBLOCK(UMM_PBLOCK(c)) & UMM_FREELIST_MASK)) {
        prevBlockSize = (c - UMM_PBLOCK(c));
    }

    /*
     * Ok, now that we're here we know how many blocks we want and the current
     * blockSize. The prevBlockSize and nextBlockSize are set and we can figure
     * out the best strategy for the new allocation as follows:
     *
     * 1. If the new block is the same size or smaller than the current block do
     *    nothing.
     * 2. If the next block is free and adding it to the current block gives us
     *    EXACTLY enough memory, assimilate the next block. This avoids unwanted
     *    fragmentation of free memory.
     *
     * The following cases may be better handled with memory copies to reduce
     * fragmentation
     *
     * 3. If the previous block is NOT free and the next block is free and
     *    adding it to the current block gives us enough memory, assimilate
     *    the next block. This may introduce a bit of fragmentation.
     * 4. If the prev block is free and adding it to the current block gives us
     *    enough memory, remove the previous block from the free list, assimilate
     *    it, copy to the new block.
     * 5. If the prev and next blocks are free and adding them to the current
     *    block gives us enough memory, assimilate the next block, remove the
     *    previous block from the free list, assimilate it, copy to the new block.
     * 6. Otherwise try to allocate an entirely new block of memory. If the
     *    allocation works free the old block and return the new pointer. If
     *    the allocation fails, return NULL and leave the old block intact.
     *
     * TODO: Add some conditional code to optimise for less fragmentation
     *       by simply allocating new memory if we need to copy anyways.
     *
     * All that's left to do is decide if the fit was exact or not. If the fit
     * was not exact, then split the memory block so that we use only the requested
     * number of blocks and add what's left to the free list.
     */

     //  Case 1 - block is same size or smaller
    if (blockSize >= blocks) 
    {
        /* This space intentionally left blank */
        //  Case 2 - block + next block fits EXACTLY
    }
    else if ((blockSize + nextBlockSize) == blocks) 
    {
        umm_assimilate_up(c);
        blockSize += nextBlockSize;

        //  Case 3 - prev block NOT free and block + next block fits
    }
    else if ((0 == prevBlockSize) && (blockSize + nextBlockSize) >= blocks) 
    {
        umm_assimilate_up(c);
        blockSize += nextBlockSize;

        //  Case 4 - prev block + block fits
    }
    else if ((prevBlockSize + blockSize) >= blocks) 
    {
        umm_disconnect_from_free_list(UMM_PBLOCK(c));
        c = umm_assimilate_down(c, 0);
        memmove((void*)&UMM_DATA(c), ptr, curSize);
        ptr = (void*)&UMM_DATA(c);
        blockSize += prevBlockSize;

        //  Case 5 - prev block + block + next block fits
    }
    else if ((prevBlockSize + blockSize + nextBlockSize) >= blocks) 
    {
        umm_assimilate_up(c);
        umm_disconnect_from_free_list(UMM_PBLOCK(c));
        c = umm_assimilate_down(c, 0);
        memmove((void*)&UMM_DATA(c), ptr, curSize);
        ptr = (void*)&UMM_DATA(c);
        blockSize += (prevBlockSize + nextBlockSize);

        //  Case 6 - default is we need to realloc a new block
    }
    else 
    {
        void* oldptr = ptr;
        if ((ptr = umm_malloc_core(size))) {
            memcpy(ptr, oldptr, curSize);
            umm_free_core(oldptr);
        }
        else 
        {
            /* This space intentionally left blnk */
        }
        blockSize = blocks;
    }

    /* Now all we need to do is figure out if the block fit exactly or if we
     * need to split and free ...
     */
    if (blockSize > blocks) 
    {
        umm_split_block(c, blocks, 0);
        umm_free_core((void*)&UMM_DATA(c + blocks));
    }

    return(ptr);
}

void* umm_calloc(size_t num, size_t item_size) 
{
    void* ret;

    ret = umm_malloc((size_t)(item_size * num));

    if (ret)
        memset(ret, 0x00, (size_t)(item_size * num));

    return ret;
}

#endif

void *bin_l_alloc(void *ud, void *ptr, size_t osize, size_t nsize)
{
    if (umm_heap == NULL)
        umm_init();

    if (nsize == 0)
    {
        //umm_free(ptr);
        free(ptr);
        return nullptr;
    }
    else
        return realloc(ptr, nsize);
	    //return umm_realloc(ptr, nsize);
}
