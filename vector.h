#ifndef _gay_vector_H_
#define _gay_vector_H_

#include <cstddef>
#include "memory.h"
#include "iterator.h"
#include "algorithm.h"
#include "gay_uninitialized.h"

namespace uvwxyz
{
    namespace gay_stl
    {

        template <class T, class Alloc = gay_alloc>
        class gay_vector
        {
        public:
            typedef T           value_type;
            typedef T*          pointer;
            typedef const T*    const_pointer;
            typedef T*          iterator;
            typedef const T*    const_iterator;
            typedef gay_reverse_iterator<iterator> reverse_iterator;
            typedef const gay_reverse_iterator<iterator> const_reverse_iterator;
            typedef T&          reference;
            typedef const T&    const_reference;
            typedef std::size_t size_type;
            typedef ptrdiff_t   difference_type;

        protected:
            typedef gay_allocator<T, gay_alloc> data_allocator;

            iterator start;
            iterator finish;
            iterator end_of_storage;

            void fill_initialize(size_type n, const_reference v)
            {
                start = allocate_and_fill(n, v);
                finish = start + n;
                end_of_storage = start + n;
            }

            void deallocate()
                {   if(start)   data_allocator::deallocate(start, finish - start);  }

            void insert_aux(iterator pos, const_reference x);

        public:
            // default constructor
            gay_vector() : start(nullptr), finish(nullptr), end_of_storage(nullptr) {}
            gay_vector(size_type n, const_reference v) {     fill_initialize(n, v);  }
            gay_vector(int       n, const_reference v) {     fill_initialize(n, v);  }
            gay_vector(long      n, const_reference v) {     fill_initialize(n, v);  }
            explicit gay_vector(size_type n)                    {     fill_initialize(n, T());    }
            explicit gay_vector(int       n)                    {     fill_initialize(n, T());    }
            explicit gay_vector(long      n)                    {     fill_initialize(n, T());    }

            ~gay_vector()
            {
                destroy(start, finish);
                deallocate();
            }

            bool operator==(const gay_vector& rhs)
            {
                if(size() == rhs.size())
                {
                    const size_type total_size = size();
                    for(size_type i = 0; i < total_size; ++i)
                        if(operator[](i) != rhs[i])
                            return false;
                    return true;
                }
                return false;
            }

            template <class InputIterator>
            gay_vector&
            assign(InputIterator first, InputIterator last)
            {
                if(!empty())
                    clear();
                for(; first != last; ++first)
                    push_back(*first);
            }

            iterator begin() const  { return start; }
            iterator end()  const   { return finish; }
            reverse_iterator rbegin() const { return reverse_iterator(end()); }
            reverse_iterator rend() const   { return reverse_iterator(begin()); }

            size_type size() const  { return size_type(finish - start); }
            size_type capacity()  const { return size_type(end_of_storage - start); }
            bool    empty() const   {   return finish == start; }

            reference operator[](const size_t n) {  return *(begin() + n);  }
            const reference operator[](const size_t n)  const   {   return *(begin() + n);  }
            const_reference front() const   {   return *begin();  }
            const_reference back() const    {   return *(end() - 1);    }

            void pop_back()
            {
                --finish;
                destroy(finish);
            }
            void push_back(const value_type& x)
            {
                if(finish != end_of_storage)
                {
                    construct(finish, x);
                    ++finish;
                }
                else
                    insert_aux(end(), x);
            }
            iterator erase(iterator first, iterator last)
            {
                iterator i = copy(last, finish, first);
                destroy(i, finish);
                finish = finish - (last - first);
                return i;
            }
            iterator erase(iterator pos)
            {
                if(pos + 1 != end())
                    copy(pos + 1, finish, pos);
                --finish;
                destroy(finish);
                return pos;
            }
            void clear()
            {   erase(start, finish); }
            // insert n T before pos.
            void insert(iterator pos, size_type n, const T& x);
            void insert(iterator pos, const T& x)   {   insert(pos, 1, x);  }

        protected:
            iterator allocate_n(size_type n)
            {
                return data_allocator::allocate(n);
            }

            iterator allocate_and_fill(size_type n, const_reference v)
            {
                iterator result = allocate_n(n);
                uninitialized_fill_n(result, n, v);
                return result;
            }

        };


        template <class T, class Alloc>
        void gay_vector<T, Alloc>::insert_aux(iterator pos, const_reference x)
        {
            // if there's space left.
            if(finish != end_of_storage)
            {
                construct(&*finish, *(finish - 1));
                ++finish;
                copy_backward(pos, finish - 2, finish - 1);
                *pos = x;
            }
            else
            {
                size_type old_size = size();
                size_type new_size = (old_size == 0) ? 1 : old_size * 2;
                iterator new_start = data_allocator::allocate(new_size);
                iterator new_finish = new_start;

                try
                {
                    // copy elements before pos
                    new_finish = uninitialized_copy(start, pos, new_start);
                    // insert element into the place.
                    construct(&*new_finish, x);
                    ++new_finish;
                    // copy elements after position.
                    new_finish = uninitialized_copy(pos, finish, new_finish);
                }
                catch(...)
                {
                    // follow commit or rollback
                    destroy(new_start, new_finish);
                    data_allocator::deallocate(new_start, new_size);
                    throw;
                }

                // destroy all elements.
                destroy(begin(), end());
                // free memory
                deallocate();

                start = new_start;
                finish = new_finish;
                end_of_storage = start + new_size;
            }
        }

        template <class T, class Alloc>
        void gay_vector<T, Alloc>::insert(iterator pos, size_type n, const T& x)
        {
            if(n == 0) return;
            // if spare space is enough
            if(size_type(end_of_storage - finish) >= n)
            {
                const size_type ele_after = finish - pos;
                iterator old_finish = finish;
                if(ele_after > n)
                {
                    // move n elements to finish.
                    uninitialized_copy(finish - n, finish, finish);
                    advance(finish, n);
                    copy_backward(pos, old_finish - n, old_finish);
                    fill(pos, pos + n, x);
                }
                else
                {
                    uninitialized_fill_n(finish, n - ele_after, x);
                    advance(finish, n - ele_after);
                    uninitialized_copy(pos, old_finish, finish);
                    // ele_after = n - (n - ele_after)
                    advance(finish, ele_after);
                    fill(pos, pos + n, x);
                }
            }
            else
            {
                size_type old_size = size();
                size_type new_size = (old_size == 0) ? 1 : old_size * 2;
                iterator new_start = data_allocator::allocate(new_size);
                iterator new_finish = new_start;

                try
                {
                    // copy elements before pos
                    new_finish = uninitialized_copy(start, pos, new_start);
                    // insert element into the place.
                    uninitialized_fill_n(new_finish, n, x);
                    ++new_finish;
                    // copy elements after position.
                    new_finish = uninitialized_copy(pos, finish, new_finish);
                }
                catch(...)
                {
                    // commit or rollback
                    destroy(new_start, new_finish);
                    data_allocator::deallocate(new_start, new_size);
                    throw;
                }

                // destroy all elements.
                destroy(begin(), end());
                // free memory
                deallocate();

                start = new_start;
                finish = new_finish;
                end_of_storage = start + new_size;
            }
        }
    }
    
}

#endif // _gay_vector_H_
