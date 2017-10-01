
#include <cstdlib> // for malloc, free
#include "gay_alloc.h"

namespace uvwxyz
{
    namespace gay_stl
    {
        // definitions of static members.
        gay_alloc::chunk* gay_alloc::freeList[gay_alloc::NODEFREE] = { 0 };
        char* gay_alloc::startOfPool    = nullptr;
        char* gay_alloc::endOfPool      = nullptr;
        size_t gay_alloc::sizeOfHeap    = 0;

        void* gay_alloc::refill(size_t n)
        {
            size_t nobjs = MAXCOLUMNS;
            char* chunks = chunk_alloc(n, nobjs);

            if(1 == nobjs)
                return chunks;

            chunk** mFreeList = freeList + INDEX(n);
            chunk   *curObj  = nullptr,
                    *nextObj = nullptr,
                    *result  = (chunk*)chunks; // the first chunk for client.
            *mFreeList = nextObj = (chunk*)(chunks + n);

            for(size_t i = 1; ; i++) // insert chunks into free list.
            {
                curObj = nextObj;
                nextObj = (chunk*)(((char*)(curObj)) + n);
                if(nobjs - 1 == i) // 19 chunks insert complete. another one will be returned to client.
                {
                    curObj->next = nullptr;
                    break;
                }
                else
                {
                    curObj->next = nextObj;
                }
            }
            return result;
        }

        char* gay_alloc::chunk_alloc(size_t sz, size_t& nobjs)
        {
            char* result = nullptr;
            size_t totalBytes = sz * nobjs,
                   bytesLeft  = endOfPool - startOfPool; // bytes left in memory pool.

            if(bytesLeft >= totalBytes) // if memory pool is rich for gay_allocating.
            {
                result = startOfPool;
                startOfPool += totalBytes;

                return result;
            }
            else if(bytesLeft >= sz) // sz <= memory pool < totalBytes
            {
                nobjs = bytesLeft / sz;
                totalBytes = sz * nobjs;
                result = startOfPool;
                startOfPool += totalBytes;

                return result;
            }
            else // memory pool is poor for gay_allocating such many bytes.
            {
                if(bytesLeft > 0)
                {
                    chunk** mFreeList = freeList + INDEX(bytesLeft);
                    ((chunk*)startOfPool)->next = *mFreeList;
                    *mFreeList = (chunk*)startOfPool;
                }

                size_t bytesInNeed = 2 * totalBytes + ROUND_UP(sizeOfHeap >> 4);
                startOfPool = (char*)malloc(bytesInNeed);

                if(!startOfPool) // fail in requiring memory from heap.
                {
                    // search for the chunk that haven't been used.
                    for(size_t i = sz; i <= MAXBYTES; i += ALIGN)
                    {
                        chunk** mFreeList = freeList + INDEX(i),
                                *curChunk = *mFreeList;
                        if(curChunk)
                        {
                            *mFreeList = curChunk->next;
                            // pump memory from free list into the memory pool
                            // memory pool must be empty.
                            startOfPool = (char*)curChunk;
                            endOfPool = startOfPool + i;

                            return chunk_alloc(sz, nobjs);
                        }
                    }
                }

                sizeOfHeap += bytesInNeed;
                endOfPool = startOfPool + bytesInNeed;

                return chunk_alloc(sz, nobjs);
            }
        }

        // public interfaces:
        void* gay_alloc::allocate(size_t sz)
        {
            if(sz > MAXBYTES)
                return malloc(sz);

            chunk* result = nullptr;
            chunk** mFreeList = freeList + INDEX(ROUND_UP(sz));

            if(*mFreeList == nullptr)
            {
                return refill(ROUND_UP(sz));
            }
            result = *mFreeList;
            *mFreeList = result->next;

            return result;
        }

        void gay_alloc::deallocate(void* ptr, size_t sz)
        {
            if(sz > MAXBYTES)
                return free(ptr);

            chunk** mFreeList = freeList + INDEX(ROUND_UP(sz));
            chunk*  first = *mFreeList;

            ((chunk*)(ptr))->next = first;
            *mFreeList = (chunk*)ptr;
        }

        void* gay_alloc::reallocate(void* ptr, size_t old_sz, size_t new_sz)
        {
            deallocate(ptr, old_sz);
            ptr = allocate(new_sz);
            return ptr;
//            chunk** mFreeList = freeList + INDEX(ROUND_UP(new_sz));
//            if(!(*mFreeList))
//                return refill(ROUND_UP(new_sz));
//
//            chunk* result = *mFreeList;
//            *mFreeList = (*mFreeList)->next;
//            return result;
        }
    }
}
