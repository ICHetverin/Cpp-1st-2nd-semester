#include <iostream>

/*
* Allocators and memory management
----------------------------------
    класс который явл промежуточным между контейнером и new

*   Container -> Allocator -> operator new -> malloc -> OS

* PoolAllocator
    сначала выдеяет большой массив, а дальше отдает по кусочка
* StackAllocator
    new не вызывает, все делает на стэке
* FreeListAllocator

* Allocator-aware containers
    неоднозначность при копировании (копировать аллокатор или делать новый)
    * select_on_container_copy_construction
    Возвращает А (в зависимости от того че хотим)
    либо копия, либо новый аллокатор
    по умолчанию - копия аллокатора (указывает в ту же память)

    аналогичный вопрос при присваивании
    * propogate_on_container_copy_assignment
    true or false
    true -> присвоить аллокатор
----------------------------------
* Operators new and delete overloading
    new состоит из двух частей: (OPERATOR NEW)
        - выделение памяти (можно переопределить) (FUNCTION NEW)
        - вызов конструктора (нельзя переопределить)
*/

//* Allocator

template <typename T>
struct allocator {
    T* allocate(size_t count) {
        // return reinterpret_cast<T*>(new char[count * sizeof(T)]);
        return operator new(count * sizeof(T));
    }

    void deallocate(T* ptr, size_t) {
        // delete[] reinterpret_cast<char*>(ptr);
        operator delete(ptr);
    }

    template<typename U, typename... Args>
    void construct(U* ptr, const Args&... args) {
        new (ptr) U(args...);
    }

    template<typename U>
    void destroy(U* ptr) {
        ptr->~U();
    }

//  Когда где-то в листе нам нужно выделить память не под инт
// а под ноду
//* typename Alloc::template rebind<Node>::other alloc;
// будет полем листа

    template <typename U>
    struct rebind {
        using other = allocator<U>;
    };

    template <typename U>
    allocator(allocator<U>) {}
// когда нужно в конструеторе вызвать конструктор аллокатора
// но нужно не инт, а какая-то кастомная структура

};

template <typename Alloc>
struct allocator_traits {

    template <typename U, typename... Args>
    void construct(Alloc& alloc, U* ptr, const Args&... args) {
        if constexpr(1 /* ... */) {
            alloc.construct(ptr, args...);
        } else {
            new (ptr) U(args...);
        }
    }
};

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
                AllocTraits::construct(alloc_, newarr+index, arr_[index]);
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

    void push_back(const T& value) {
        if (sz_ == cap_) {
            reserve(cap_ > 0 ? cap_ * 2 : 1);
        }
    }
};
//----------------------------------
//* New and delete oveloading

void* operator new(size_t n) {
    std::cout << n << " bytes allocated\n";
    return malloc(n);
};

void operator delete(void* ptr) {
    free(ptr);
}

void* operator new[](size_t n) {
    std::cout << n << "[] bytes allocated\n";
    return malloc(n);
}

void operator delete[](void* ptr) {
    free(ptr);
}

int main() {
    {
        std:;vector<int> v;
        for (int i = 0; i < 50; ++i) {
            v.push_back(i);
        }
    }
    {
        int* p = new int[100];
        delete[] p;
    }
}