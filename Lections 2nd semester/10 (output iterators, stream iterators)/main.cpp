#include <iostream>
#include <algorithm>
#include <vector>

/*
* Output iterators and stream iterators
    Output iterator - гарантирует корректность: можно разыменовывать, инкрементировать и присваиватт несколько раз
    все итераторы в контейнерах - не output

    Нужна обертка над итератором вектора, кот наз
    std::back_insert_iterator

* Streams
    * Pipe operator |
*/

bool even(int x) {
    return x%2 == 0;
}

template <typename Container>
class back_insert_iterator {
    Container& container;
public:
    back_insert_iterator(Container& container) : container(container) {}

    back_insert_iterator& operator=(const typename Container::value_type& value) {
        container.push_back(value);
        return *this;
    }

    back_insert_iterator& operator++() {
        return *this;
    }

    back_insert_iterator& operator++(int) {
        return *this;
    }

    back_insert_iterator& operator() {
        return *this;
    }

// *it1++ = *it2;
// а вот это умом, получается при разымновании и инкрементации
// ничего не происходит, а при присваивании делаем push_back
};

template<typename Container>
back_insert_iterator<Container> back_inserter(Container& container) {
    return {container};
}

int main() {
    {
        int a[10] = {1, 2, 3, 4, 5};
        std::vector<int> v;
        std::copy(a, a + 10, v.begin());
        /*
        Так нельзя написать, потому что
        UB, потому что copy просто разыменовывает
        присваивает инкрементирует
        а вот если что-то типа list, то инкремент может
        вести в рандомнуб память
        */
    }
    {
        int a[10] = {1, 2, 3, 4, 5};
        std::vector<int> v;
        std::copy_if(a, a + 10,
            std::back_insert_iterator<std::vector<int>>(v),
            &even);
    }
    {
        int a[10] = {1, 2, 3, 4, 5};
        std::vector<int> v;
        std::copy_if(a, a + 10, back_inserter(v), &even);
    }
    {

    }
}