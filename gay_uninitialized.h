#ifndef _GAYUNINITIALIZED_H_
#define _GAYUNINITIALIZED_H_

#include <iostream>
#include "iterator.h"
#include "algorithm.h"
#include "gay_construct.h"
#include "gay_type_traits.h"

using std::cout;
using std::endl;

namespace uvwxyz
{
    namespace gay_stl
    {

        template<class InputIterator, class ForwardIterator>
        ForwardIterator
        __uninitalized_copy_aux(InputIterator first, InputIterator last, ForwardIterator result,
                              __gay__true_type)
        {
            return gay_stl::copy(first, last, result);
        }

        template <class InputIterator, class ForwardIterator>
        ForwardIterator
        __uninitalized_copy_aux(InputIterator first, InputIterator last, ForwardIterator result,
                            __gayFalse_type)
        {
            ForwardIterator cur = result;
            for(; first != last; ++first, ++cur)
                construct(&*cur, *first);
            return cur;
        }

        template <class InputIterator, class ForwardIterator, class ValueType>
        ForwardIterator
        __uninitalized_copy(InputIterator first, InputIterator last, ForwardIterator result,
                            ValueType*)
        {
            typedef typename __gay__type_traits<ValueType>::is_pod pod_type;
            return __uninitalized_copy_aux(first, last, result, pod_type());
        }

        template <class InputIterator, class ForwardIterator>
        ForwardIterator
        uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result)
        {
            return __uninitalized_copy(first, last, result, gay_iterator_value_type(first));
        }

        template <class ForwardIterator, class Size, class T>
        inline ForwardIterator
        __uninitialized_fill_n_aux(ForwardIterator first, Size n, const T& x, __gay__true_type)
        {
            return gay_stl::fill_n(first, n, x);
        }

        template <class ForwardIterator, class Size, class T>
        inline ForwardIterator
        __uninitialized_fill_n_aux(ForwardIterator first, Size n, const T& x, __gayFalse_type)
        {
            T x_copy = x;
            ForwardIterator cur = first;
            for(; n > 0; --n, ++cur)
                construct(&*cur, x_copy);
            return cur;
        }

        template <class ForwardIterator, class Size, class T>
        inline ForwardIterator
        uninitialized_fill_n(ForwardIterator first, Size n, const T& x)
        {
            typedef typename __gay__type_traits<T>::is_pod pod_type;
            return __uninitialized_fill_n_aux(first, n, x, pod_type());
        }

        template <class ForwardIterator, class T>
        inline void
        __uninitialized_fill_aux(ForwardIterator first, ForwardIterator last, const T& x, __gay__true_type)
        {
            gay_stl::fill(first, last, x);
        }

        template <class ForwardIterator, class T>
        inline void
        __uninitialized_fill_aux(ForwardIterator first, ForwardIterator last, const T& x, __gayFalse_type)
        {
            ForwardIterator cur = first;
            for(; cur != last; ++cur)
                construct(&*cur, x);
        }

        template <class ForwardIterator, class T>
        inline void
        uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& x)
        {
            typedef typename __gay__type_traits<T>::is_pod pod_type;
            __uninitialized_fill_aux(first, last, x, pod_type());
        }

    }
}

#endif // _GAYUNINITIALIZED_H_
