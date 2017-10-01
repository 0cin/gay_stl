#ifndef _gay_alloc_H_
#define _gay_alloc_H_

#include <cstddef> // for size_t

using std::size_t;

namespace uvwxyz
{
    namespace gay_stl
    {

        class gay_alloc
        {
        private:
            enum bytesAlign  { ALIGN = 8 };
            enum maxBytes  { MAXBYTES = 128 };
            enum nodesOfFreeList  { NODEFREE = (MAXBYTES / ALIGN) };
            enum maxColumns  { MAXCOLUMNS = 20 };

        private:
            union chunk
            {
                chunk* next;
                char    client[1];
            };
            static chunk* freeList[NODEFREE];

        private:
            static char* startOfPool; // start position of memory pool.
            static char* endOfPool; // end position of memory pool.
            static size_t sizeOfHeap;   // size of memory pool.

        private:
            static size_t ROUND_UP(size_t n)    {   return ((n + (ALIGN - 1)) & ~(ALIGN - 1));   }
            // n is rounded up to 8.
            static size_t INDEX(size_t n)   {   return (n / ALIGN - 1); }
            // refill free list by calling chunk_alloc.
            static void* refill(size_t n);
            // pump sz * nobjs bytes from memory pool.
            // if memory pool is poor to serve then it will be refill
            // or the value of nobjs will go lower.
            static char* chunk_alloc(size_t sz, size_t& nobjs);

        public:
            static void* allocate(size_t sz);
            static void deallocate(void* ptr, size_t sz);
            static void* reallocate(void* ptr, size_t old_sz, size_t new_sz);

        };
    }
}

#endif // _gay_alloc_H_
