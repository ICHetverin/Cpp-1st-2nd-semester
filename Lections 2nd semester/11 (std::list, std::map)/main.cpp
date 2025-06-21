#include <iostream>

/*
* std::list internals
------------------------------------------------
* Methods:
    *- insert(iterator it, const T& value)  O(1)
    *- erase(iterator it)                   O(1)
    *- push_back / pop_back
    *- push_front / pop_front

    * Bidirectional iterator

    Будет fake Node, которая связана с началом и с концом - получается цикл
    она и будет beg

    * Необычные методы
    *- sort()
    *- merge()
    *- splice()
        можно взять кусок с list1 и вставить в определнное место в list2
-----------------------
* forward_list

    * No push_back / pop_back
    * No size()
    * since c++ 11
    * Forward iterator
    * No insert()
------------------------------------------------
*/

template<typename T>
class list {
    struct BaseNode {
        BaseNode* next;
        BaseNode* prev;
    };
    struct Node : BaseNode {
        T value;
    };
    // Node* head;          version 1.0
    // Node* tail;
    // BaseNode* fakeNode;  verison 2.0
    BaseNode fakeNode;   // version 3.0
    size_t sz_;

public:
    list() : fakeNode{&fakeNode, &fakeNode}, sz_(0) {}

};

/*
* std::map internals
------------------------------------------------
* Methods: all guaranteed (O(log n))
    *- pair<iterator, bool> insert(const pair<Key, Value>&)

    *- erase(Key)
    *- iterator erase(Iterator)

    *- iterator find(Key)

    *- Value& at(Key)               |
    *- const Value& at(Key) const   |-> exception
    *- Value& opertor[](Key)        |

    *- size_t count(Key) (~multimap (там не одно вхождение))

    *- iterator lower_bound(Key)
    *- iterator upper_bound(Key)

    * Bidirectional iterator
----------------
* fake Node - корень без правого ребенка
    все дерево - левый ребенок fake node
------------------------------------------------
*/

template<typename Key, typename Value, typename Compare = std::less<Key>>
class map {
    BaseNode* fakeNode;
    BaseNode* begin;
    size_t sz_;
    Compare comp;

    struct BaseNode {
        Node* left;
        Node* right;
        Node* parent;
    };

    struct Node : BaseNode {
        std::pair<const Key, Value> kv;
        bool red;
    };
};