#include <iostream>
#include <algorithm>
#include <vector>
#include <iterator>

/*
* Output iterators and stream iterators
    Output iterator - гарантирует корректность: можно разыменовывать, инкрементировать и присваиватт несколько раз
    все итераторы в контейнерах - не output

    Нужна обертка над итератором вектора, кот наз
    std::back_insert_iterator

* Streams
    * Pipe operator |

* std::optional<T>
    хранит либо Т либо ничего
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

// *it1++ = *it2;
// а вот это умом, получается при разымновании и инкрементации
// ничего не происходит, а при присваивании делаем push_back
};

template<typename Container>
back_insert_iterator<Container> back_inserter(Container& container) {
    return {container};
}

//--------------------------
//* Input iterator - istream iterator

template <typename T>
class istream_iterator {
    std::istream* in = nullptr;
    T value;
public:
    using iterator_category = std::input_iterator_tag;
    using pointer = T*;
    using value_type = T;
    using reference = T&;
    using difference_type = int;

    istream_iterator(std::istream& in) : in(&in) {
        in >> value;
    }
    istream_iterator() {}

    istream_iterator& operator++() {
        if (!*in >> value) {
            *this = istream_iterator();
        }
        return *this;
    }

    T& operator*() {
        return value;
    }
};

//* std::optional
template<typename T>
class optional {
    char value[sizeof(T)];
    bool initialized = false;
public:
    optional(const T& newv) : initialized(true) {
        new (value) T (newv);
    }
    optional () {}
    ~optional() {
        if (initialized) {
            reinterpret_cast<T*>(value)->~T();
        }
    }
    bool has_value() const {
        return initialized;
    }
    operator bool() const {
        return initialized;
    }
    T& operator*() {
        return reinterpret_cast<T&>(*value);
    }
    T& value_or(T& other) {
        return initialized ? reinterpret_cast<T&>(*value) : other;
    }
};

struct nullopt_t {};
nullopt_t nullopt;
//--------------------------

int main() {
    {
        int a[10] = {1, 2, 3, 4, 5};
        std::vector<int> v;
        // std::copy(a, a + 10, v.begin());
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
        // std::copy_if(a, a + 10, back_inserter(v), &even);
    }
    {
        std::istream_iterator<int> it(std::cin);
        std::vector<int> v;

        for (int i = 0; i < 5; ++i, ++it) {
            v.push_back(*it);
        }

        for (int i = 0; i < v.size(); ++i) {
            std::cout << v[i] << std::endl;
        }
    }
    {
        istream_iterator<int> it(std::cin);
        std::vector<int> v;

        // std::copy(it, std::istream_iterator<int>(), v.begin());
        // std::copy_n(it, 10, v.begin());
        // std::copy(it, std::istream_iterator<int>(), std::back_inserter(v));

        for (int i = 0; i < v.size(); ++i) {
            std::cout << v[i] << std::endl;
        }
    }
}