#include <iostream>

/*
* std::move std::forward implementation and push_back using move-semantic
    1) Args - lvalue (T&) - (cast) -> T&
    2) Args - rvalue (T) - (cast) -> T&&
        CE
    Смысл forward lv -> lv | rv -> rv (прокинуть дальше)

*/

//* std::forward implementation

template <typename T>
T&& forward(T& value) {
    return static_cast<T&&>(value);
}

// исправление

template <typename T>
T&& forward(std::remove_reference_t<T>& value) {
    return static_cast<T&&>(value);
}

// 2nd overloading
template <typename T>
T&& forward(std::remove_reference_t<T>&& value) {
    static_assert(!std::is_lvalue_reference_v<T>);
    return static_cast<T&&>(value);
}

//* std::move implementation
// старое

template <typename T>
T&& move(T& value) {
    return static_cast<T&&>(value);
}
//! проблема, если T - int& допустим
// то на выходе мув будет его же и отдавать как lvalue
// мы хотим чтобы мув всегда отдавать rvalue

// новое

template <typename T>
std::remove_reference_t<T>&& move(T&& value) {
    return static_cast<std::remove_reference_t<T>&&>(value);
}

//* push_back implementation with move-semantic
// есть пример push_back(v[3]);
// поэтому в стл сначала кладется новый элемент
// а затем перекладываются другие

template <typename T, typename Alloc = std::allocator<T>>
class vector {
    T* arr_;
    size_t sz_;
    size_t cap_;
    Alloc alloc_;

    using AllocTraits = std::allocator_traits<Alloc>;

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

    vector& operator=(const vector& other) {
        // а теперь добавим проверку для полноты условий
        Alloc newalloc = AllocTraits::propogate_on_container_copy_assignment::value
            ? other.alloc_ : alloc_;

        // это все было сделано, если мы хотим оставить наш аллокатор
        T* newarr = AllocTraits::allocate(newalloc, other.cap_);
        size_t i = 0;
        try {
            for (; i < other.sz_; ++i) {
                AllocTraits::construct(newalloc, newarr + i, other[i]);
            }
        } catch (...) {
            for (size_t j = 0; j < i; ++j) {
                AllocTraits::destroy(newalloc, newarr + j);
            }
            AllocTraits::deallocate(newalloc, newarr, other.cap_);
            throw;
        }

        for (size_t i = 0; i < sz_; ++i) {
            AllocTraits::destroy(alloc_, arr_ + i);
        }
        AllocTraits::deallocate(alloc_, arr_, cap_);
        // Важно что свои конструкции нужно удалить своим старым аллокатором

        arr_ = newarr;
        sz_ = other.sz_;
        cap_ = other.cap_;
        alloc_ = newalloc;
    }

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

        // T* newarr = reinterpret_cast<T*>(new char([newcap * sizeof(T)]));
        T* newarr = AllocTraits::allocate(alloc_, newcap);

        size_t index = 0;
        try {
            for (; index < sz_; ++index) {
                // new(newarr + index) T(arr_[index]);
                AllocTraits::construct(alloc_, newarr+index, std::move_if_noexcept(arr_[index]));
                // если мув конструктор не no_except тогда lvalue и копируем, иначе rvalue и муваем
                //* поэтому всегда помечаем мув как no_except
            }
        } catch (...) {
            for (size_t oldindex = 0; oldindex < index; ++oldindex) {
                // (newarr + oldindex)->~T();
                AllocTraits::destroy(alloc_, newarr+oldindex);
            }
            // delete[] reinterpret_cast<char*>(newarr);
            AllocTraits::deallocate(alloc_, newarr, newcap);
            throw;
        }

        for (size_t index = 0; index < sz_; ++index) {
            // (arr + index)->~T();
            AllocTraits::destroy(alloc_, arr_+index);
        }
        // delete[] reinterpret_cast<char*>(arr_);
        AllocTraits::deallocate(alloc_, arr_, cap_);

        arr_ = newarr;
        cap_ = newcap;
    }

    // old
    // void push_back(const T& value) {
    //     if (sz_ == cap_) {
    //         reserve(cap_ > 0 ? cap_ * 2 : 1);
    //     }
    //     AllocTraits::construct(alloc_, arr_ + sz_, value);
    //     ++sz;
    // }

    // new
    void push_back(const T& value) {
        emplace_back(value);
    }

    // old
    // void push_back(T&& value) {
    //     if (sz_ == cap_) {
    //         reserve(cap_ > 0 ? cap_ * 2 : 1);
    //     }
    //     AllocTraits::construct(alloc_, arr_ + sz_, std::move(value);
    //     ++sz_;
    // }

    // new
    void push_back(T&& value) {
        emplace_back(std::move(value));
    }

    template <typename... Args>
    void emplace_back(Args&&... args) {
        if (sz_ == cap_) {
            reserve(cap_ > 0 ? cap_ * 2 : 1);
        }
        AllocTraits::construct(alloc_, arr_ + sz_,
            std::forward<Args>(args)...);
        ++sz_;
    }
};