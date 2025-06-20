#include <iostream>

/*
* Iterators implementation, const- and reverse- iterators
    std::reverse_iterator
    -> and <-
*/

template <typename Iterator>
typename std::iterator_traits<Iterator>::diference_type distance(Iterator first, Iterator last) {
    if constexpr (std::is_base_of<
        typename std::iterator_traits<Iterator>::iterator_category,
        std::random_access_iterator_tag
        >) {
        return last - first;
    }

    int i = 0;
    for (; first != last; ++i, ++first);
    return i;
}


template <typename T>
class vector {
    T* arr_;
    size_t sz_;
    size_t cap_;

/*
* Некоторый кринж из-за того что вылезает идентичное определение
* двух разных итераторов
* обычного и того, который указывает на константные объекты под собой
* как нормальным людям нужно сделать шаблонно
public:
    class const_iterator {
    private:
        const T* ptr
        iterator(T* ptr) : ptr(ptr) {};
    public:
        iterator(const iterator&) = default;
        iterator& operator=(const iterator&) = default;

        const T& operator*() { return *ptr; }
        const T* operator->() const { return ptr; }
        iterator& operator++() { ++ptr; return *this; }
        iterator operator++(int) {
            iterator copy = *this;
            ++ptr;
            return copy;
        }

    };

    class iterator {
    private:
        T* ptr
        iterator(T* ptr) : ptr(ptr) {};
    public:
        iterator(const iterator&) = default;
        iterator& operator=(const iterator&) = default;

        T& operator*() { return *ptr; }
        T* operator->() const { return ptr; }
        iterator& operator++() { ++ptr; return *this; }
        iterator operator++(int) {
            iterator copy = *this;
            ++ptr;
            return copy;
        }
*/
private:
    template<bool IsConst>
    class base_iterator {
    public:
        using pointer_type = std::conditional_t<IsConst, const T*, T*>;
        using reference_type = std::conditional_t<IsConst, const T&, T&>;
        using value_type = T;

    private:
        pointer_type ptr;
        base_iterator(T* ptr) : ptr(ptr) {};

        friend class vector<T>;

    public:
        base_iterator(const base_iterator&) = default;
        base_iterator& operator=(const base_iterator&) = default;

        reference_type operator*() { return *ptr; }
        pointer_type operator->() const { return ptr; }
        base_iterator& operator++() { ++ptr; return *this; }
        base_iterator operator++(int) {
            base_iterator copy = *this;
            ++ptr;
            return copy;
        }

        operator base_iterator<true>() const {
            return {ptr};
        }
    };

public:
    using iterator = base_iterator<false>;
    using const_iterator = base_iterator<true>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    iterator begin() { return {arr_}; }
    iterator end() { return {arr_ + sz_}; }

    const_iterator begin() const { return {arr_}; }
    const_iterator end() const { return {arr_ + sz_}; }

    const_iterator cbegin() const { return {arr_}; }
    const_iterator cend() const { return {arr_ + sz_}; }

    void reserve(size_t newcap) {
        if (newcap <= cap_) {
            return;
        }

        T* newarr = reinterpret_cast<T*>(new char([newcap * sizeof(T)]));

        size_t index = 0;
        try {
        // T* newarr = new T[newcap]                                       1.0 version (1 ошибка)
            for (; index < sz_; ++index) {
            //  newarr[index] = arr_[index];                               2.0 version (2 ошибка) - UB (объект еще не создан, там лежат сырые байты)
                new(newarr + index) T(arr_[index]);                     // 3.0 version (placement new) (может быть исключения в конструкторе Т)
            }
        } catch (...) {
            for (size_t oldindex = 0; oldindex < index; ++oldindex) {
                (newarr + oldindex)->~T();
            }
            delete[] reinterpret_cast<char*>(newarr);
            throw;
        }

        // delete[] arr_;                                                  4.0 version (3 ошибка)
        for (size_t index = 0; index < sz_; ++index) {
            (arr + index)->~T();                                        // 5.0 verison (вручную вызываем деструктор)
        }
        delete[] reinterpret_cast<char*>(arr_);                         // 6.0 version (reinterpret cast)

        arr_ = newarr;
        cap_ = newcap;
    }

    void push_back(const T& value) {
        if (sz_ == cap_) {
            reserve(cap > 0 ? cap_ * 2 : 1);
        }
    }
};

int main() {
    {
        std::vector<int> v(10);
        int& x = v[5];
        v.push_back(1);
        std::cout << x;
        //! UB
    }
    {
        std::vector<int> v(10);
        std::vector<int>::iterator x = v.begin() + 5;
        v.push_back(1);
        std::cout << *x;
        //! UB
        // в целом могли бы хранить указатель на контейнер
        // но это было бы дороже в разыменовании (дважды)
        // + не соблюдется требование о:
        // при swape контейнеров
        // все итераторы и ссылки остаются валидными
    }
}