#ifndef _GAYgay_allocATOR_H_
#define _GAYgay_allocATOR_H_

#include <cstddef> // for size_t
#include "gay_alloc.h"

namespace uvwxyz
{
    namespace gay_stl
    {
        // wrap alloc for allocating type bytes once.
        template<class Type, class Alloc = gay_alloc>
        class gay_allocator
        {
        public:
            typedef Type value_type;
            typedef size_t size_type;

        public:
            // allocate n * sizeof(value_type) for once.
            static value_type* allocate(size_type n)
            {
                return (n) ? (value_type*)(Alloc::allocate(n * sizeof(value_type))) : nullptr;
            }
            // allocate a piece of memory for one object to hold.
            static value_type* allocate(void)
            {
                return (value_type*)(Alloc::allocate(sizeof(value_type)));
            }
            static void deallocate(value_type* ptr, size_type n)
            {
                if(ptr && n)
                    Alloc::deallocate(ptr, n * sizeof(value_type));
            }
            static void deallocate(value_type* ptr)
            {
                if(ptr)
                    Alloc::deallocate(ptr, sizeof(value_type));
            }
            static value_type* reallocate(value_type* ptr, size_type old_n, size_type new_n)
            {
                if(ptr && old_n && new_n)
                    return Alloc::reallocate(ptr, old_n * sizeof(value_type), new_n * sizeof(value_type));
                return nullptr;
            }
        };
    }
}

#endif // _GAYgay_allocATOR_H_
