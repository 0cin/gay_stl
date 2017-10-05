#ifndef _gay_iterator_H_
#define _gay_iterator_H_

#include <cstddef> // for ptrdiff_t

namespace uvwxyz
{
    namespace gay_stl
    {
        // read-only iterator
        struct gay_input_iterator_tag    {};
        // write-only iterator
        struct gay_output_iterator_tag   {};
        // iterator that only goes forward
        struct gay_forward_iterator_tag : public gay_input_iterator_tag {};
        // iterator that can go left & right
        struct gay_bidirectional_iterator_tag : public gay_forward_iterator_tag {};
        // iterator that almost equal to a  pointer
        struct gay_random_access_iterator_tag : public gay_bidirectional_iterator_tag   {};

        // the base of all invalid standard iterators
        // declare the attributes that a iterator must has.
        template< class Category,
                 class T,
                 class Distance = ptrdiff_t,
                 class Pointer = T*,
                 class Reference = T& >
        struct  gay_iterator
        {
            typedef Category    iterator_category;
            typedef T           value_type;
            typedef Distance    difference_type;
            typedef Pointer     pointer;
            typedef Reference   reference;
        };

        template<class Type, class Distance = ptrdiff_t>
        struct  gayInput_iterator : public gay_iterator<gay_input_iterator_tag, Type, Distance>   {};

        template<class Type, class Distance = ptrdiff_t>
        struct  gayOutput_iterator : public gay_iterator<gay_output_iterator_tag, Type, Distance, void, void>  {};

        template<class Type, class Distance = ptrdiff_t>
        struct  gayForward_iterator : public gay_iterator<gay_forward_iterator_tag, Type, Distance>   {};

        template<class Type, class Distance = ptrdiff_t>
        struct  gayBidirectional_iterator : public gay_iterator<gay_bidirectional_iterator_tag, Type, Distance>   {};

        template<class Type, class Distance = ptrdiff_t>
        struct  gayRandom_access_iterator : public gay_iterator<gay_random_access_iterator_tag, Type, Distance>    {};

        // IteratorType is a kind of iterator that extends gay_iterator
        template<class IteratorType>
        struct  __gay_iterator_traits
        {
            typedef typename IteratorType::iterator_category    iterator_category;
            typedef typename IteratorType::value_type           value_type;
            typedef typename IteratorType::difference_type      difference_type;
            typedef typename IteratorType::pointer              pointer;
            typedef typename IteratorType::reference            reference;
        };

        // Specialization for random access iterators.
        template<class Type>
        struct  __gay_iterator_traits<Type*>
        {
            typedef gay_random_access_iterator_tag   iterator_category;
            typedef Type                            value_type;
            typedef ptrdiff_t                       difference_type;
            typedef Type*                           pointer;
            typedef Type&                           reference;
        };

        template<class Type>
        struct  __gay_iterator_traits<const Type*>
        {
            typedef gay_random_access_iterator_tag   iterator_category;
            typedef Type                            value_type;
            typedef ptrdiff_t                       difference_type;
            typedef const Type*                     pointer;
            typedef const Type&                     reference;
        };

        template <class Iterator>
        class gay_reverse_iterator : public gay_iterator <
            typename __gay_iterator_traits<Iterator>::iterator_category,
            typename __gay_iterator_traits<Iterator>::value_type,
            typename __gay_iterator_traits<Iterator>::difference_type,
            typename __gay_iterator_traits<Iterator>::pointer,
            typename __gay_iterator_traits<Iterator>::reference >
        {
        protected:
            Iterator current;
            typedef __gay_iterator_traits<Iterator> traits_type;

        public:
            typedef Iterator iterator_type;
            typedef typename traits_type::difference_type difference_type;
            typedef typename traits_type::pointer   pointer;
            typedef typename traits_type::reference reference;


            gay_reverse_iterator() : current()   { }
            explicit
            gay_reverse_iterator(iterator_type x) : current(x) { }
            gay_reverse_iterator(const gay_reverse_iterator& x)
                : current(x.current) { }
            template <class U>
            gay_reverse_iterator(const gay_reverse_iterator<U>& x)
                : current(x.current) { }

            iterator_type base() const {  return current; }

            reference operator*() const
            {
                iterator_type tmp = current;
                return *(--tmp);
            }

            pointer operator->() const { return &(operator*()); }

            gay_reverse_iterator& operator++()
            {
                --current;
                return *this;
            }

            gay_reverse_iterator operator++(int)
            {
                gay_reverse_iterator tmp(*this);
                --current;
                return tmp;
            }

            gay_reverse_iterator& operator--()
            {
                ++current;
                return *this;
            }

            gay_reverse_iterator operator--(int)
            {
                gay_reverse_iterator tmp(*this);
                ++current;
                return tmp;
            }

            gay_reverse_iterator& operator+=(difference_type n)
            {
                current -= n;
                return *this;
            }

            gay_reverse_iterator& operator-=(difference_type n)
            {
                current += n;
                return *this;
            }

            gay_reverse_iterator& operator+(difference_type n) const { return gay_reverse_iterator(current - n); }
            gay_reverse_iterator& operator-(difference_type n) const { return gay_reverse_iterator(current + n); }
            reference operator[](difference_type n) const { return *(*this + n); }
        };

        template <class Iterator>
        inline bool
        operator==(const gay_reverse_iterator<Iterator>& x,
               const gay_reverse_iterator<Iterator>& y)
        { return x.base() == y.base(); }

        template <class Iterator>
        inline bool
        operator!=(const gay_reverse_iterator<Iterator>& x,
                const gay_reverse_iterator<Iterator>& y)
        { return !(x == y); }

        template <class Iterator>
        inline bool
        operator<(const gay_reverse_iterator<Iterator>& x,
                const gay_reverse_iterator<Iterator>& y)
        { return x.base() < y.base(); }

        template <class Iterator>
        inline bool
        operator>(const gay_reverse_iterator<Iterator>& x,
                const gay_reverse_iterator<Iterator>& y)
        { return x.base() > y.base(); }

        template <class Iterator>
        inline bool
        operator<=(const gay_reverse_iterator<Iterator>& x,
                const gay_reverse_iterator<Iterator>& y)
        { return(x < y || x == y); }

        template <class Iterator>
        inline bool
        operator>=(const gay_reverse_iterator<Iterator>& x,
                const gay_reverse_iterator<Iterator>& y)
        { return(x > y || x == y); }

        // get the category of a iterator.
        template<class IteratorType>
        inline typename __gay_iterator_traits<IteratorType>::iterator_category
        gay_iterator_category(const IteratorType&)
        {
            typedef typename __gay_iterator_traits<IteratorType>::iterator_category category;
            return category();
        }

        template<class IteratorType>
        inline typename __gay_iterator_traits<IteratorType>::value_type*
        gay_iterator_value_type(const IteratorType&)
        {
            typedef typename __gay_iterator_traits<IteratorType>::value_type value;
            return static_cast<value*>(nullptr);
        }

        template<class IteratorType>
        inline typename __gay_iterator_traits<IteratorType>::difference_type*
        gay_iterator_difference_type(const IteratorType&)
        {
            typedef typename __gay_iterator_traits<IteratorType>::difference_type difference;
            return static_cast<difference*>(nullptr);
        }
    }
}

#endif // _gay_iterator_H_
