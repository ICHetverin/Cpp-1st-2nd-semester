/*
------------------------------------------------
* std::list internals
------------------------------------------------
* Methods:
    *- insert(iterator it, const T& value)  O(1)
    *- erase(iterator it)                   O(1)
    *- push_back / pop_back
    *- push_front / pop_front

    * Bidirectional iterators

    Будет fake Node, которая связана с началом и с концом - получается цикл
    она и будет beg

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
    BaseNode* fakeNode;
    size_t sz_;
};