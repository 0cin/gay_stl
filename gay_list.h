#ifndef _GAYLIST_H_
#define _GAYLIST_H_

#include "memory.h"
#include "gay_iterator.h"
#include "algorithm.h"
#include "gay_construct.h"

namespace uvwxyz
{
    namespace gay_stl
    {
        template <class T>
        struct __gay_list_node
        {
            typedef __gay_list_node<T>* pointer_type;
            pointer_type    prev;
            pointer_type    next;
            T               data;
        };

        template <class T, class Ref, class Pointer>
        struct __gay_list_iterator :
                public gay_iterator <gay_bidirectional_iterator_tag, T>
        {
            // iterator of list is bidirectional iterators.
            typedef gay_bidirectional_iterator_tag  iterator_category;
            typedef T                               value_type;
            typedef Ref                             reference;
            typedef Pointer                         pointer;
            typedef ptrdiff_t                       difference_type;
            typedef __gay_list_node<T>*             link_type;
            typedef size_t                          size_type;

            typedef __gay_list_iterator<T, Ref, Pointer>    self;
            typedef __gay_list_iterator<T, T&,  T*>         iterator;

            link_type node; // what iterator points.

            __gay_list_iterator() {}
            __gay_list_iterator(link_type x) : node(x) {}
            __gay_list_iterator(const iterator& x) : node(x.node) {}
            self&   operator=(const iterator& x) {  node = x.node;  return *this;   }
            self&   operator=(link_type x) {    node = x;   return *this;   }

            bool operator==(const self& another) const {    return node == another.node;   }
            bool operator!=(const self& another) const {    return node != another.node;   }

            reference operator*() const {   return node->data;   }
            reference operator->() const {  return &(operator*());  }

            self& operator++()
            {
                node = (link_type)((*node).next);
                return *this;
            }

            self operator++(int)
            {
                self tmp = *this;
                ++*this;
                return tmp;
            }

            self& operator--()
            {
                node = (link_type)((*node).prev);
                return *this;
            }

            self operator--(int)
            {
                self tmp = *this;
                --*this;
                return tmp;
            }
        };

        template <class T, class Alloc = gay_alloc>
        class gay_list
        {
        protected:
            typedef __gay_list_node<T> list_node;

        public:
            typedef list_node*                              link_type;
            typedef T&                                      reference;
            typedef const T&                                const_reference;
            typedef T                                       value_type;
            typedef __gay_list_iterator<T, T&, T*>          iterator;
            typedef const __gay_list_iterator<T, T&, T*>    const_iterator;
            typedef gay_reverse_iterator<iterator>          reverse_iterator;
            typedef const gay_reverse_iterator<iterator>    const_reverse_iterator;
            typedef typename iterator::size_type            size_type;
            // allocator for allocating nodes.
            typedef gay_allocator<list_node, Alloc>         gay_list_node_allocator;

        protected:
            // node is a node that take the charator
            link_type node;
            // allocate a node.
            link_type get_node() { return gay_list_node_allocator::allocate(); }
            void put_node(link_type ptr) {   gay_list_node_allocator::deallocate(ptr); }

            // create and initialize a node
            link_type create_node(const_reference x)
            {
                link_type pnode = get_node();
                construct(&((*pnode).data), x);
                return pnode;
            }
            void destroy_node(link_type ptr)
            {
                destroy(&((*ptr).data));
                put_node(ptr);
            }
            void empty_initialize()
            {
                node = get_node();
                node->next = node;
                node->prev = node;
            }
            void transfer(iterator pos, iterator first, iterator last)
            {
                // make forward link.
                ((link_type)(last.node))->prev->next = pos.node;
                ((link_type)(first.node))->prev->next = last.node;
                ((link_type)(pos.node))->prev->next = first.node;
                iterator tmp = ((link_type)(pos.node))->prev;
                // make reverse link.
                ((link_type)(pos.node))->prev = ((link_type)(last.node))->prev;
                ((link_type)(last.node))->prev = ((link_type)(first.node))->prev;
                ((link_type)(first.node))->prev = tmp.node;
            }

        public:
            gay_list()
            {   empty_initialize(); }
            template <class ForwardIterator>
            gay_list(ForwardIterator first, ForwardIterator last)
            {
                empty_initialize();
                assign(first, last);
            }
            gay_list(const gay_list<T>& another)
            {
                empty_initialize();
                assign(another.begin(), another.end());
            }
            ~gay_list()
            {   clear(); }

            iterator begin() const {   return (link_type)((*node).next);    }
            iterator end()  const {   return (link_type)(node);    }
            const_iterator cbegin() const {  return (link_type)((*node).next);  }
            const_iterator cend()   const { return (link_type)(node);    }
            const_reference front() const { return *(begin());   }
            const_reference back() const {  return *(end());    }
            reverse_iterator rbegin() const {   return reverse_iterator(end()); }
            reverse_iterator rend() const { return reverse_iterator(begin());   }
            const_reverse_iterator crbegin() const {    return reverse_iterator(end());   }
            const_reverse_iterator crend()  const { return reverse_iterator(begin());   }

            template <class ForwardIterator>
            gay_list&
            assign(ForwardIterator first, ForwardIterator last)
            {
                for(; first != last; ++first)
                    push_back(*first);
                return *this;
            }

            bool empty() const {    return (*node).next == node;    }
            size_type size() const {    return distance(begin(), end());    }
            // insert x before pos
            iterator insert(iterator pos, const_reference x)
            {
                link_type new_node = create_node(x);
                new_node->next = pos.node;
                new_node->prev = pos.node->prev;
                ((link_type)((pos.node)->prev))->next = new_node;
                (pos.node)->prev = new_node;

                return new_node;
            }
            void push_back(const_reference x)   {   insert(end(), x);   }
            void push_front(const_reference x)  {   insert(begin(), x); }

            iterator erase(iterator pos)
            {
                link_type next_node = (link_type)((pos.node)->next);
                ((link_type)(pos.node))->prev->next = next_node;
                ((link_type)(pos.node))->next->prev = ((link_type)(pos.node))->prev;

                destroy_node(pos.node);
                return next_node;
            }
            // erase all the node in range[first, last)
            iterator erase(iterator first, iterator last)
            {
                iterator scan = first;
                while(scan != last)
                {
                    iterator next = ((link_type)(next.node))->next;
                    erase(first);
                    first = scan;
                }
                return last;
            }
            void pop_front()    {   erase(begin()); }
            void pop_back()
            {
                // the true tail node is the node before end
                iterator tmp = end();
                --tmp;
                erase(tmp);
            }
            void clear()
            {
                link_type start = node->next,
                          next  = nullptr;
                while(start != node)
                {
                    next = start->next;
                    destroy_node(start);
                    start = next;
                }
                node->next = node;
                node->prev = node;
            }
            // remove all the nodes that data == v
            void remove(const_reference v)
            {
                link_type start = node->next,
                          next = nullptr;
                while(start != node)
                {
                    next = start->next;
                    if(start->data == v)
                        erase(iterator(start));
                    start = next;
                }
            }
            void unique()
            {
                link_type first = node->next,
                          next = first->next;
                // if list is empty.
                if(first == node)   return;
                while(++next != node)
                {
                    if(next->data == first->data)
                        erase(next);
                    else
                        first = next;
                    next = first;
                }
            }
            void splice(iterator pos, const gay_list<T, Alloc>& l)
            {   if(!l.empty()) transfer(pos, l.begin(), l.end());   }
            void splice(iterator pos, iterator first, iterator last)
            {
                if(first != last)
                    transfer(pos, first, last);
            }
            void splice(iterator pos, gay_list&, iterator i)
            {
                iterator j = i;
                ++j;
                if(pos == i || pos == j) return;
                transfer(pos, i, j);
            }
            void swap(gay_list<T, Alloc>& x)
            {
                gay_list<T, Alloc> tmp;
                tmp.splice(tmp.begin(), x);
                x.splice(x.begin(), *this);
                splice(begin(), tmp);
            }
            void reverse()
            {
                if(empty()) return;
                iterator scan = node->next;
                ++scan;
                while(scan.node != node)
                {
                    iterator next = scan;
                    ++next;
                    splice(iterator(node->next), *this, scan);
                    scan = next;
                }
            }
            // *this and x must be sorted.
            void merge(gay_list<T, Alloc>& x)
            {
                iterator first = begin(), last  = end(),
                         xfirst = x.begin(), xlast = x.end();

                while(first != last && xfirst != xlast)
                {
                    if(*xfirst < *first)
                    {
                        iterator next = xfirst;
                        transfer(first, xfirst, ++next);
                        xfirst = next;
                    }
                    else
                    {
                        ++first;
                    }
                }
                if(xfirst != xlast)
                    transfer(last, xfirst, xlast);
            }
            // it's merge sort without recursiveness
            void sort()
            {
                // do nothing if list is empty or has only 1 element.
                if(node->next == node || node->next->next == node)
                    return;
                gay_list<T, Alloc> carry;
                // counter takes the place of function calling stack.
                // counter[63] can hold 2^64 - 1 elements.
                // it's enough to treat most of sort problem in general.
                gay_list<T, Alloc> counter[64];
                int __fill = 0;

                while(!empty())
                {
                    carry.splice(carry.begin(), *this, begin());
                    int i = 0;
                    while(i < __fill && !counter[i].empty())
                    {
                        counter[i].merge(carry);
                        carry.swap(counter[i++]);
                    }
                    carry.swap(counter[i]);
                    if(i == __fill)
                        ++__fill;
                }
                // merge all the nodes in count[__fill - 1] as result.
                for(int i = 1; i < __fill; ++i)
                    counter[i].merge(counter[i - 1]);
                swap(counter[__fill - 1]);
            }
        };
    }
}

#endif // _GAYLIST_H_
