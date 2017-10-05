#include <typeinfo>
#include <iostream>
#include <cstring>
#include <string>
#include "gay_uninitialized.h"
#include "algorithm.h"
#include "memory.h"
#include "iterator.h"
#include "gay_deque.h"
#include "gay_vector.h"
#include "gay_stack.h"
#include "gay_queue.h"
#include "list.h"
#include <iterator>
#include <algorithm>
#include <vector>

using namespace std;
using namespace uvwxyz;
using namespace gay_stl;

union column
{
    column* next;
    char    client[1];
};

struct p
{
    int a;
    int b;
    p() : a(0), b(-1) {}
};

int main()
{
//    char mem[100];
//    column* pc = (column*)mem;
//    cout << (void*)(mem) << endl << pc << endl << (void*)(pc->client) << endl;
//    strcpy((char*)(pc), "ffff");
//    cout << mem << endl;

    column* mColumn = gay_stl::gay_allocator<column>::allocate(6);
    for(int i = 0; i < 5; i++)
        (mColumn + i)->next = (mColumn + i + 1);
    char*   chunk = gay_stl::gay_allocator<char>::allocate(50);
    memset(chunk, 'f', 50 * sizeof(char));
    cout << chunk << endl;

    //cout << typeid(gay_stl::gayOutput_iterator<int>::pointer).name() << endl;
    //cout << typeid(gay_stl::gayForward_iterator<int>::pointer).name() << endl;

    int iTest[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int iResult[100];
    gay_stl::uninitialized_copy(iTest, iTest + 10, iResult);

    for(int i = 0; i < 10; i++)
        cout << iResult[i] << " ";
    cout << endl;

    gay_stl::gay_vector<int> f(10, 88);
    for(int i = 0; i < 5; ++i)
        f.push_back(i);
    auto it = f.begin();
    while(it != f.end())
    {
        cout << *it << " "
        ;++it;
    }
    f.erase(f.end() - 1);
    cout << f.back() << endl;
    cout << endl << f.size() << endl << f.capacity() << endl;
    gay_stl::gay_vector<p> f2(22);
    for(auto itr : f2)
        cout << itr.a << " " << itr.b << endl;
    cout << endl;

    int iBackward[100];
    gay_stl::copy_backward(iTest, iTest + 10, iBackward + 10);
    for(int i = 9; i >= 0; --i)
        cout << iBackward[i] << " ";
    cout << endl;


    f.insert(f.begin(), 233);
    it = f.begin();
    while(it != f.end())
    {
        cout << *it << " "
        ;++it;
    }
    cout << endl;

    gay_vector<int>::reverse_iterator ritr = f.rbegin();
    while(ritr != f.rend())
        cout << *(ritr++) << " ";
    cout << endl;

    f.clear();
    cout << f.size() << endl;

    vector<int> vtrA;
    for(int i = 1; i <= 10; i++)
        vtrA.push_back(i);

    auto pos = find(vtrA.begin(), vtrA.end(), 3);

    vector<int>::reverse_iterator rpos(pos);
    cout << *rpos << endl;

    gay_list<int> m_gaylist;
    cout << "--------------------------gay_list test-------------------------" << endl;
    for(int i = 0; i < 10; ++i)
        m_gaylist.push_back(i + 1);
    gay_list<int>::iterator list_itr = m_gaylist.begin();
    for(; list_itr != m_gaylist.end(); ++list_itr)
        cout << *list_itr << " ";
    cout << endl
         << m_gaylist.size() << endl;

    gay_list<int>::iterator points_3 = m_gaylist.begin();
    gay_list<int>::iterator points_5 = points_3;
    for(int i = 0; i < 2; ++i)
        points_3++;
    for(int i = 0; i < 5; ++i)
        points_5++;
    cout << *points_3 << endl << *points_5 <<endl;
    m_gaylist.splice(m_gaylist.begin(), points_3, points_5);
    list_itr = m_gaylist.begin();
    for(; list_itr != m_gaylist.end(); ++list_itr)
        cout << *list_itr << " ";
    cout << endl;

    gay_list<int>::reverse_iterator rlist_itr = m_gaylist.rbegin();
    while(rlist_itr != m_gaylist.rend()) {
        cout << *rlist_itr << " ";
        ++rlist_itr;
    }
    cout << endl;

    m_gaylist.erase(points_3);
    m_gaylist.erase(points_5);
    list_itr = m_gaylist.begin();
    for(; list_itr != m_gaylist.end(); ++list_itr)
        cout << *list_itr << " ";
    cout << endl;

    m_gaylist.sort();
    list_itr = m_gaylist.begin();
    for(; list_itr != m_gaylist.end(); ++list_itr)
        cout << *list_itr << " ";
    cout << endl;

    int *ip = new int[50];
    gay_stl::fill(ip, ip + 50, 6);
    int *ip2 = new int[50];
    gay_stl::copy(ip, ip + 50, ip2);
    for(int i = 0; i < 50; ++i)
        cout << ip2[i] << " ";
    cout << endl;

    int **iip = new int*[10];
    for(int i = 0; i < 10; ++i)
    {
        *(iip + i) = new int[5];
        for(int j = 0; j < 5; ++j)
            iip[i][j] = 5;
    }
    int **iip2 = new int*[10];
    for(int i = 0; i < 10; ++i)
        *(iip2 + i) = new int[5];
    int **iip3 = new int*[10];
    for(int i = 0; i < 10; ++i)
        *(iip3 + i) = new int[5];
    std::copy(iip, iip + 10, iip3);
    gay_stl::copy(iip, iip + 10, iip2);
    for(int i = 0; i < 10; ++i)
    {
        for(int j = 0; j < 5; ++j)
            cout << iip2[i][j] << " ";
        cout << endl;
    }
    cout << endl;
    for(int i = 0; i < 10; ++i)
    {
        for(int j = 0; j < 5; ++j)
            cout << iip3[i][j] << " ";
        cout << endl;
    }
    cout << endl;
    for(int i = 0; i < 10; ++i)
    {
        delete[] *(iip + i);
    }
    delete []iip;
    for(int i = 0; i < 10; ++i)
    {
        for(int j = 0; j < 5; ++j)
            cout << iip2[i][j] << " ";
        cout << endl;
    }
    cout << endl;
    for(int i = 0; i < 10; ++i)
    {
        for(int j = 0; j < 5; ++j)
            cout << iip3[i][j] << " ";
        cout << endl;
    }

    gay_stl::gay_deque<int, gay_stl::gay_alloc, 8> m_deque;
    //for(int i = 0; i < 10; ++i)
        //m_deque.push_back(i * 10);
    for(int i = 0; i < 10; ++i)
        m_deque.push_front(i * 100);
    typedef decltype(m_deque)::iterator deque_iterator;
    for(deque_iterator itr = m_deque.begin(); itr != m_deque.end(); ++itr)
        cout << *itr << " ";
    cout << endl;
    for(int i = 0; i < 5; ++i)
        m_deque.pop_front();
    for(deque_iterator itr = m_deque.begin(); itr != m_deque.end(); ++itr)
        cout << *itr << " ";
    cout << endl;
    m_deque.insert(m_deque.insert(m_deque.begin() + 1, 2333), 23336);
    for(deque_iterator itr = m_deque.begin(); itr != m_deque.end(); ++itr)
        cout << *itr << " ";
    cout << endl;

    int* ip4 = (int*)malloc(10 * sizeof(int));
    for(int i = 0; i < 10; ++i)
        *(ip4 + i) = i + 1;
    for(int i = 0; i < 10; ++i)
        cout << ip4[i] << " ";
    cout << endl;
    //free(ip4 + 1);
    for(int i = 0; i < 10; ++i)
        cout << ip4[i] << " ";
    cout << endl;
    m_deque.clear();
    gay_stl::gay_deque<int> m_deque2(100);

    gay_stl::gay_stack<int> m_stack;
    for(int i = 0; i < 10; ++i)
        m_stack.push(i + 5);
    for(int i = 0; i < 10; ++i)
    {
        cout << m_stack.top() << endl;
        m_stack.pop();
    }
    cout << endl;
    gay_stl::gay_queue<int> m_queue;
    for(int i = 0; i < 10; ++i)
        m_queue.push(i + 20);
    for(int i = 0; i < 10; ++i)
    {
        cout << m_queue.front() << " " << m_queue.back() << endl;
        m_queue.pop();
    }
    cout << endl;

    return 0;
}
