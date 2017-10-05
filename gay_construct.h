#ifndef _GAYCONSTRUCT_H_
#define _GAYCONSTRUCT_H_

#include "gay_type_traits.h"

namespace uvwxyz
{
    namespace gay_stl
    {
        template<class T1, class T2>
        inline void construct(T1* ptr, const T2& value)
        {
            new(ptr) T1(value);
        }

        template<class T>
        inline void destroy(T* ptr)
        {
            ptr->~T();
        }

        template<class ForwardIterator>
        inline void _destroy(ForwardIterator first, ForwardIterator last, __gay_true_type)  {   }

        template<class ForwardIterator>
        inline void _destroy(ForwardIterator first, ForwardIterator last, __gay_false_type)
        {
            for(; first != last; ++first)
                destroy(&*first);
        }

        // destroy in range[first, last)
        template<class ForwardIterator>
        inline void destroy(ForwardIterator first, ForwardIterator last)
        {
            typedef typename __gay__type_traits<ForwardIterator>::is_pod ispod;
            _destroy(first, last, ispod());
        }

        inline void destroy(char*, char*)   {}
        inline void destroy(wchar_t*, wchar_t*) {}
    }
}

#endif // _GAYCONSTRUCT_H_
