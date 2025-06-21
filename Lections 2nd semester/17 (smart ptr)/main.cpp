#include <iostream>

/*
* RAII (smart pointers)

* unique_ptr
    simple

* shared_ptr


*/

template <typename T>
struct default_delete {
    void operator()(T* ptr) const {
        delete ptr;
    }
};

//* unique_ptr implementation

template <typename T, typename Deleter = std::default_delete<T>>
class unique_ptr {
    T* ptr;
    [[no_unique_address]] Deleter del;

public:
    unique_ptr(T* ptr, Deleter del) : ptr(ptr), del(del) {}
    unique_ptr(T* ptr) : ptr(ptr) {}
    ~unique_ptr() {
        delete ptr;
    }
    unique_ptr(const unique_ptr&) = delete;
    unique_ptr& operator=(const unique_ptr&) = delete;

    unique_ptr(unique_ptr&& other) : ptr(other.ptr), del(other.del) {
        other.ptr = nullptr;
    }
    unique_ptr& operator=(unique_ptr&& other) {
        if (this != other) {
            del(ptr);
            ptr = other.ptr;
            del = std::move(other.del);
            other.ptr = nullptr;
        }
        return *this;
    }

    T& operator*() const {
        return *ptr;
    }
    T* operator->() const {
        return ptr;
    }

    T* get() const {
        return ptr;
    }
};

//* shared_ptr implementation

template <typename T>
class shared_ptr {
private:
    T* ptr;
    ControlBlock* count;

    struct ControlBlock{
        size_t shared_count;
        size_t weak_count;
    };

    struct ControlBlockWithObject : ControlBlock {
        T value;
    };

    template <typename U>
    friend shared_ptr<U> make_shared(Args&&...)

    shared_ptr(ControlBlock* cp); //TODO

public:
    shared_ptr(T* ptr) : ptr(ptr), count(new size_t(1)) {}

    shared_ptr(const shared_ptr& other) : ptr(ptr), count(count) {
        ++*count;
    }

    ~shared_ptr() {
        if(!count) return;

        --*count;
        if (!*count) {
            // delete count;
            delete ptr;
        }
    }
};

//* weak ptr implementation

//* make_shared, make_unique implementation

template <typename T, typename... Args>
unique_ptr<T> make_unique(Args&&... args) {
    return unique_ptr<T>(new T(std::forward<Args>(args)...));
}

template <typename T, typename... Args>
shared_ptr<T> make_shared(Args&&... args) {
    auto* p = new shared_ptr<T>::ControlBlock{T(std::forward<args>(args)...), 1};
    return shared_ptr<T>(p);
}