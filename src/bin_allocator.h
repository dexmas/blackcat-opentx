#ifndef _BIN_ALLOCATOR_H_
#define _BIN_ALLOCATOR_H_

#include "debug.h"

template <int SIZE_SLOT, int NUM_BINS> class BinAllocator 
{
private:
    PACK(struct Bin {
        char data[SIZE_SLOT];
        char Used;
    });
    struct Bin Bins[NUM_BINS];
    int NoUsedBins;
public:
    BinAllocator() : NoUsedBins(0) {
        memclear(Bins, sizeof(Bins));
    }
    bool free(void* ptr) {
        for (size_t n = 0; n < NUM_BINS; ++n) {
            if (ptr == Bins[n].data) {
                Bins[n].Used = false;
                --NoUsedBins;
                return true;
            }
        }
        return false;
    }
    bool is_member(void* ptr) {
        return (ptr >= Bins[0].data && ptr <= Bins[NUM_BINS - 1].data);
    }
    void* malloc(size_t size) {
        if (size > SIZE_SLOT) {
            return 0;
        }
        if (NoUsedBins >= NUM_BINS) {
            return 0;
        }
        for (size_t n = 0; n < NUM_BINS; ++n) {
            if (!Bins[n].Used) {
                Bins[n].Used = true;
                ++NoUsedBins;
                return Bins[n].data;
            }
        }
        return 0;
    }
    size_t size(void* ptr) {
        return is_member(ptr) ? SIZE_SLOT : 0;
    }
    bool can_fit(void* ptr, size_t size) {
        return is_member(ptr) && size <= SIZE_SLOT;  //todo is_member check is redundant
    }
    unsigned int capacity() { return NUM_BINS; }
    unsigned int size() { return NoUsedBins; }
};

#if defined(SIMU)
typedef BinAllocator<39, 300> BinAllocator_slots1;
typedef BinAllocator<79, 100> BinAllocator_slots2;
#else
typedef BinAllocator<31, 512> BinAllocator_slots1;
typedef BinAllocator<63, 128> BinAllocator_slots2;
#endif

#if defined(USE_BIN_ALLOCATOR)
extern BinAllocator_slots1 slots1;
extern BinAllocator_slots2 slots2;

// wrapper for our BinAllocator for Lua
void* bin_l_alloc(void* ud, void* ptr, size_t osize, size_t nsize);
#endif //USE_BIN_ALLOCATOR

#endif // _BIN_ALLOCATOR_H_
