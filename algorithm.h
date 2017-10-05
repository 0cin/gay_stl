#ifndef _GAYALGORITHM_H_
#define _GAYALGORITHM_H_

#include <cstring> // for operation of memory
#include "gay_iterator.h"
#include "gay_type_traits.h"

namespace uvwxyz
{
    namespace gay_stl
    {
        template <class T>
        const T& max(const T& lhs, const T& rhs)
        {
            return(lhs < rhs) ? rhs : lhs;
        }
        template <class T>
        const T& min(const T& lhs, const T& rhs)
        {
            return(rhs < lhs) ? rhs : lhs;
        }

        template <class InputIterator>
        typename __gay_iterator_traits<InputIterator>::difference_type
        __distance(InputIterator first, InputIterator last, gay_input_iterator_tag)
        {
            typename __gay_iterator_traits<InputIterator>::difference_type n = 0;
            while(first != last)
                ++first, ++n;
            return n;
        }

        template <class InputIterator>
        typename __gay_iterator_traits<InputIterator>::difference_type
        __distance(InputIterator first, InputIterator last, gay_random_access_iterator_tag)
        {
            return last - first;
        }

        template <class InputIterator>
        typename __gay_iterator_traits<InputIterator>::difference_type
        distance(InputIterator first, InputIterator last)
        {
            typedef typename __gay_iterator_traits<InputIterator>::iterator_category category;
            return __distance(first, last, category());
        }

        template <class RandomAccessIterator, class Distance>
        void
        __advance(RandomAccessIterator& input, Distance distance, gay_random_access_iterator_tag)
        {
            input += distance;
        }

        template <class BidirectionalIterator, class Distance>
        void
        __advance(BidirectionalIterator& input, Distance distance, gay_bidirectional_iterator_tag)
        {
            if(distance > 0)
                while(distance--)
                    ++input;
            else
                while(distance++)
                    --input;
        }

        template <class InputIterator, class Distance>
        void
        __advance(InputIterator& input, Distance distance, gay_input_iterator_tag)
        {
            while(distance--)
                ++input;
        }

        template <class InputIterator, class Distance>
        void
        advance(InputIterator& input, Distance distance)
        {
            __advance(input, distance, gay_iterator_category(input));
        }

        // copy for pod type
        template <class RandomAccessIterator, class OutputIterator>
        OutputIterator
        __copy_aux_(RandomAccessIterator first, RandomAccessIterator last, OutputIterator result, __gay_true_type)
        {
            auto dis = distance(first, last);
            memcpy(result, first, dis * sizeof(*first));
            advance(result, dis);
            return result;
        }

        template <class RandomAccessIterator, class OutputIterator>
        OutputIterator
        __copy_aux_(RandomAccessIterator first, RandomAccessIterator last, OutputIterator result, __gay_false_type)
        {
            for(; first != last; ++result, ++first)
                *result = *first;
            return result;
        }

        template <class RandomAccessIterator, class OutputIterator, class ValueType>
        OutputIterator
        __copy_aux(RandomAccessIterator first, RandomAccessIterator last, OutputIterator result, ValueType*)
        {
            typedef typename __gay__type_traits<ValueType>::is_pod pod_type;
            return __copy_aux_(first, last, result, pod_type());
        }

        template <class RandomAccessIterator, class OutputIterator>
        OutputIterator
        __copy(RandomAccessIterator first, RandomAccessIterator last, OutputIterator result, gay_random_access_iterator_tag)
        {
            return __copy_aux(first, last, result, gay_iterator_value_type(first));
        }

        template <class ForwardIterator, class OutputIterator>
        OutputIterator
        __copy(ForwardIterator first, ForwardIterator last, OutputIterator result, gay_forward_iterator_tag)
        {
            for(; first != last; ++result, ++first)
                *result = *first;
            return result;
        }

        // return result + (last - first)
        template <class InputIterator, class OutputIterator>
        OutputIterator
        copy(InputIterator first, InputIterator last, OutputIterator result)
        {
            typedef typename __gay_iterator_traits<InputIterator>::iterator_category category;
            // call __copy to handle
            return __copy(first, last, result, category());
        }

        template <class ForwardIterator, class Size, class T>
        ForwardIterator
        fill_n(ForwardIterator first, Size n, const T& v)
        {
            ForwardIterator cur = first;
            for(; n > 0; --n, ++cur)
                *cur = v;
            return cur;
        }

        // Assigns x to all the elements in the range [first,last).
        template <class ForwardIterator, class T>
        void fill(ForwardIterator first, ForwardIterator last, const T& x)
        {
            ForwardIterator cur = first;
            for(; cur != last; ++cur)
                *cur = x;
        }

        // Copies the elements in the range [first,last) starting from the end into the range terminating at result.
        template <class BidirectionalIterator1, class BidirectionalIterataor2>
        BidirectionalIterataor2
        copy_backward(BidirectionalIterator1 first, BidirectionalIterator1 last,
                        BidirectionalIterataor2 result)
        {
            while(first != last)
                *(--result) = *(--last);
            return result;
        }
    }
}

#endif // _GAYALGORITHM_H_
