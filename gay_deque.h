#ifndef _GAYDEQUE_H_
#define _GAYDEQUE_H_

#include <cstddef>
#include "memory.h"
#include "gay_iterator.h"
#include "gay_construct.h"
#include "gay_uninitialized.h"

using std::size_t;

namespace uvwxyz
{
    namespace gay_stl
    {
        inline size_t __deque_buf_size(size_t n, size_t sz)
        {
            return n != 0 ? n : (sz < 512 ? size_t(512 / sz) : size_t(1));
        }

        template <class T, class Ref, class Ptr, size_t BufSize>
        struct __gay_deque_iterator :
            public gay_iterator< gay_random_access_iterator_tag, T >
        {
            typedef __gay_deque_iterator<T, T&, T*, BufSize>    iterator;
            typedef __gay_deque_iterator<T, const T&, const T*, BufSize> const_iterator;

            typedef T   value_type;
            typedef Ref reference;
            typedef Ptr pointer;
            typedef ptrdiff_t difference_type;

            typedef T** map_pointer;
            typedef size_t size_type;

            static  size_type buffer_size() { return (size_type)(BufSize);    }

            typedef __gay_deque_iterator self;

            T* cur; // points to the current of the buffer.
            T* first; // points to the begin of the buffer.
            T* last;  // points to the end of buffer including spare space.
            map_pointer node;

            // set current node as new node.
            void set_node(map_pointer new_node)
            {
                node = new_node;
                first = *node;
                last = first + (difference_type)(buffer_size());
            }

            __gay_deque_iterator() :
                cur(nullptr), first(nullptr), last(nullptr), node(nullptr) {}
            __gay_deque_iterator(T* x, map_pointer y) :
                cur(x), first(*y), last(*y + buffer_size()), node(y) {}
            __gay_deque_iterator(const __gay_deque_iterator& another) :
                cur(another.cur), first(another.first), last(another.last), node(another.node)  {}

            reference operator*() const {   return *cur;  }
            pointer operator->() const {    return &(operator*()); }
            difference_type operator-(const self& x) const
            {
                return difference_type(buffer_size()) * (node - x.node - 1) + (cur - first) + (x.last - x.cur);
            }
            self& operator++()
            {
                ++cur;
                if(cur == last)
                {
                    set_node(node + 1);
                    cur = first;
                }
                return *this;
            }
            self operator++(int)
            {
                self tmp = *this;
                ++this;
                return tmp;
            }
            self& operator--()
            {
                if(cur == first)
                {
                    set_node(node - 1);
                    cur = last;
                }
                --cur;
                return *this;
            }
            self operator--(int)
            {
                self tmp = *this;
                --this;
                return tmp;
            }

            self& operator+=(difference_type n)
            {
                difference_type offset = n + (cur - first);
                if(offset >= 0 && offset <= difference_type(buffer_size()))
                {
                    cur += n;
                }
                else
                {
                    difference_type node_offset = (offset > 0) ? offset / difference_type(buffer_size())
                                                            : -difference_type((-offset - 1) / buffer_size()) - 1;
                    set_node(node + node_offset);
                    cur = first + (offset - node_offset * difference_type(buffer_size()));
                }
                return *this;
            }
            self operator+(difference_type n) const
            {
                self tmp = *this;
                tmp += n;
                return tmp;
            }
            self operator-(difference_type n) const
            {
                self tmp = *this;
                tmp += - n;
                return tmp;
            }

            reference operator[](difference_type __n) const {   return *(*this + __n);  }

            bool operator==(const self& x) const    {   return x.cur == cur;    }
            bool operator!=(const self& x) const    {   return cur != x.cur;  }
            bool operator<(const self& x) const     {   return (node == x.node) ? cur < x.cur : (node < x.node);    }
        };

        template <class T, class Alloc = gay_alloc, size_t BufSize = 0>
        class gay_deque
        {
        public:
            typedef T                                       value_type;
            typedef T*                                      pointer;
            typedef const T*                                const_pointer;
            typedef T&                                      reference;
            typedef const T&                                const_reference;
            typedef size_t                                  size_type;
            typedef ptrdiff_t                               difference_type;
            typedef __gay_deque_iterator<T, reference, pointer, BufSize>
                                                            iterator;
            typedef __gay_deque_iterator<T, const_reference, const_pointer, BufSize>
                                                            const_iterator;
            typedef gay_reverse_iterator<iterator>          reverse_iterator;
            typedef gay_reverse_iterator<const_iterator>    const_reverse_iterator;

        protected:
            typedef pointer* map_pointer;
            typedef gay_allocator<value_type, Alloc> data_allocator;
            typedef gay_allocator<pointer, Alloc> map_allocator;

            static size_type buffer_size()
            {
                return __deque_buf_size(BufSize, sizeof(value_type));
            }
            static size_type initial_map_size()
            {   return 8;   }

        protected:
            map_pointer map;
            size_type   map_size;

            iterator start, finish;

            pointer allocate_node()
            {
                return data_allocator::allocate(buffer_size());
            }
            void deallocate_node(pointer p)
            {
                data_allocator::deallocate(p, buffer_size());
            }
            // n is the number of elements we need.
            // a node includes buffer_size() elements.
            void create_map_and_nodes(size_type n)
            {
                size_type num_nodes = n / buffer_size() + 1;
                size_type map_size_ = max(initial_map_size(), num_nodes + 2);
                // allocate a map which hold map_size nodes.
                map_pointer map = map_allocator::allocate(map_size_);
                map_pointer nstart = map + (map_size_ - num_nodes) / 2,
                            nfinish = nstart + num_nodes - 1,
                            cur = nullptr;

                try
                {
                    for(cur = nstart; cur <= nfinish; ++cur)
                        *cur = allocate_node();
                }
                catch(...)
                {
                    while(nstart < cur || nstart == cur)
                    {
                        deallocate_node(*cur);
                        --cur;
                    }
                }
                start.set_node(nstart);
                start.cur = start.first;
                finish.set_node(nfinish);
                finish.cur = finish.first + n % buffer_size();
                map_size = map_size_;
            }
            void fill_initialize(size_type n, const value_type& v)
            {
                create_map_and_nodes(n);
                map_pointer cur = nullptr;
                try
                {
                    for(cur = start.node; cur < finish.node; ++cur)
                        gay_stl::uninitialized_fill(*cur, *cur + buffer_size(), v);
                    gay_stl::uninitialized_fill(finish.first, finish.cur, v);
                }
                catch(...)
                {
                    // deep¡ádark¡áfantasy
                }
            }
            template <class InputIterator>
            void fill_initialize(InputIterator first, InputIterator last)
            {
                create_map_and_nodes(gay_stl::distance(first, last));
                map_pointer cur = nullptr;
                try
                {
                    gay_stl::copy(first, last, start);
                }
                catch(...)
                {

                }
            }
            void reallocate_map(size_type nodes_to_add, bool add_at_front)
            {
                size_type old_num_nodes = finish.node - start.node + 1,
                          new_num_nodes = old_num_nodes + nodes_to_add;

                map_pointer new_nstart = nullptr;

                if(map_size > 2 * new_num_nodes)
                {
                    new_nstart = map + (map_size - new_num_nodes) +
                                    (add_at_front ? nodes_to_add : 0);
                    if(new_nstart < start.node)
                        gay_stl::copy(start.node, finish.node + 1, new_nstart);
                    else
                        gay_stl::copy_backward(start.node, finish.node + 1, new_nstart + old_num_nodes);
                }
                else
                {
                    size_type new_map_size = map_size + max(map_size, new_num_nodes) + 2;
                    map_pointer new_map = map_allocator::allocate(new_map_size);
                    new_nstart = map + (map_size - new_num_nodes) +
                                    (add_at_front ? nodes_to_add : 0);

                    gay_stl::copy(start.node, finish.node + 1, new_nstart);

                    map_allocator::deallocate(map, map_size); // release the old map
                    map = new_map;
                    map_size = new_map_size;
                }
                start.set_node(new_nstart);
                finish.set_node(new_nstart + old_num_nodes - 1);
            }
            void reserve_map_at_back(size_type nodes_to_add = 1)
            {
                if(map_size - (finish.node - map) - 1 < nodes_to_add)
                    reallocate_map(nodes_to_add, false);
            }
            void reserve_map_at_front(size_type nodes_to_add = size_type(1))
            {
                if((start.node - map) < difference_type(nodes_to_add))
                    reallocate_map(nodes_to_add, true);
            }
            void push_back_aux(const value_type& x)
            {
                value_type x_copy = x;
                reserve_map_at_back();
                *(finish.node + 1) = allocate_node();
                try
                {
                    gay_stl::construct(finish.cur, x_copy);
                    finish.set_node(finish.node + 1);
                    finish.cur = finish.first;
                }
                catch(...)
                {
                    deallocate_node(*(finish.node + 1));
                }
            }
            void push_front_aux(const value_type& x)
            {
                value_type x_copy = x;
                reserve_map_at_front();
                *(start.node - 1) = allocate_node();
                try
                {
                    start.set_node(start.node - 1);
                    start.cur = start.last - 1;
                    gay_stl::construct(start.cur, x_copy);
                }
                catch(...) // rollback
                {
                    start.set_node(start.node + 1);
                    start.cur = start.first;
                    deallocate_node(*(start.node - 1));
                    throw;
                }
            }
            void pop_back_aux()
            {
                deallocate_node(finish.first);
                finish.set_node(finish.node - 1);
                finish.cur = finish.last - 1;
                destroy(finish.cur);
            }
            void pop_front_aux()
            {
                destroy(start.cur);
                deallocate_node(start.first);
                start.set_node(start.node + 1);
                start.cur = start.first;
            }
            iterator insert_aux(iterator pos, const_reference x)
            {
                difference_type elems_before = pos - start;
                value_type x_copy = x;

                if(elems_before < difference_type(size() / 2))
                {
                    push_front(front());    // call push_front in order to reallocate a map which can hold all the elements.
                    iterator front1 = start;
                    ++front1;
                    iterator front2 = front1;
                    ++front2;
                    pos = start + elems_before;
                    iterator pos1 = pos;
                    ++pos1;
                    gay_stl::gay_forward_iterator_tag p;
                    gay_stl::__copy(front2, pos1, front1, p);
                }
                else
                {
                    push_back(back());
                    iterator back1 = finish;
                    --back1;
                    iterator back2 = back1;
                    --back2;
                    pos = start + elems_before;
                    gay_stl::copy_backward(pos, back2, back1);
                }
                *pos = x_copy;
                return pos;
            }

        public:
            gay_deque() :
                map(nullptr), map_size(0), start(), finish() {
                create_map_and_nodes(0);
            }
            gay_deque(int n) { create_map_and_nodes(n); }
            gay_deque(size_type n) { create_map_and_nodes(n); }
            gay_deque(int n, const_reference x) { fill_initialize(n, x); }
            gay_deque(size_type n, const_reference x) { fill_initialize(n, x); }

            iterator begin()    {  return start;  }
            iterator end()  {   return finish;  }
            const_iterator begin() const  {   return start;   }
            const_iterator end() const  {   return finish; }
            reverse_iterator rbegin()   {   return reverse_iterator(end()); }
            reverse_iterator rend() {   return reverse_iterator(begin());   }
            const_reverse_iterator rbegin() const { return const_iterator(end());   }
            const_reverse_iterator rend() const {   return const_iterator(begin()); }

            reference operator[](size_type n)   {   return start[n];    }
            const_reference operator[](size_type n) const { return start[n];    }
            const_reference front() const { return *start;  }
            const_reference back() const {
                iterator tmp = finish;
                --tmp;
                return *tmp;
            }
            reference front() { return *start; }
            reference back() {
                iterator tmp = finish;
                --tmp;
                return *tmp;
            }
            size_type size() const  {   return finish - start; }
            size_type max_size() const {    return size_type(-1);   }
            bool    empty() const { return start == finish;    }

            void push_back(const value_type& x)
            {
                // if there're space left.
                if(finish.cur + 1 != finish.last)
                {
                    gay_stl::construct(finish.cur, x);
                    ++finish.cur;
                }
                else
                    push_back_aux(x);
            }
            void push_front(const value_type& x)
            {
                // if there're space left
                if(start.cur != start.first)
                {
                    gay_stl::construct(start.cur - 1, x);
                    --start.cur;
                }
                else
                    push_front_aux(x);
            }
            void pop_back()
            {
                // the last buffer is not empty
                if(finish.cur != finish.first)
                {
                    --finish.cur;
                    gay_stl::destroy(finish.cur);
                }
                else
                    pop_back_aux();
            }
            void pop_front()
            {
                if(start.cur + 1 != start.last)
                {
                    gay_stl::destroy(start.cur);
                    ++start.cur;
                }
                else
                    pop_front_aux();
            }
            void clear()
            {
                for(map_pointer scan = start.node + 1; scan != finish.node; ++scan)
                {
                    // destroy all the elements in a buffer.
                    gay_stl::destroy(*scan, *scan + buffer_size());
                    // delete the buffer.
                    deallocate_node(*scan);
                }
                // the first and the last buffer are not empty.
                if(start.node != finish.node)
                {
                    gay_stl::destroy(start.cur, start.last);
                    //deallocate(start.first);
                    gay_stl::destroy(finish.first, finish.cur);
                    deallocate_node(finish.first);
                }
                // there's only one buffer which is not empty
                else
                    gay_stl::destroy(start.cur, finish.cur);
                finish = start;
            }
            iterator erase(iterator pos)
            {
                iterator next = pos;
                ++next;
                difference_type index = pos - start;
                if(index < (size() >> 1)) // if elements before pos is less than behind.
                {
                    gay_stl::copy_backward(start, pos, next);
                    pop_front();
                }
                else
                {
                    gay_stl::copy(next, finish, pos);
                    pop_back();
                }
                return start + index;
            }
            // erase all the elements in range[first, last)
            iterator erase(iterator first, iterator last)
            {
                if(first == start && last == finish)
                {
                    clear();
                    return finish;
                }
                else
                {
                    difference_type n = last - first,
                                    elems_before = first - start;
                    if(elems_before < ((size() - n) >> 1))
                    {
                        gay_stl::copy_backward(start, first, last);
                        iterator new_start = start + n;
                        gay_stl::destroy(start, new_start);

                        for(map_pointer scan = start.node; scan != new_start.node; ++scan)
                            data_allocator::deallocate(*scan, buffer_size());
                        start = new_start;
                    }
                    else
                    {
                        copy(last, finish, first);
                        iterator new_finish = finish - n;
                        gay_stl::destroy(new_finish, finish);

                        for(map_pointer scan = new_finish.node + 1; scan <= finish.node; ++scan)
                            data_allocator::deallocate(*scan, buffer_size());
                        finish = new_finish;
                    }
                    return start + elems_before;
                }
            }
            iterator insert(iterator pos, const_reference x)
            {
                if(pos == start)
                {
                    push_front(x);
                    return start;
                }
                else if(pos == finish)
                {
                    push_back(x);
                    iterator tmp = finish;
                    --tmp;
                    return tmp;
                }
                else
                {
                    return insert_aux(pos, x);
                }

            }
        };
    }
}

#endif // _GAYDEQUE_H_
