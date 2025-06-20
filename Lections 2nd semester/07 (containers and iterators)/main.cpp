#include <iostream>

/*
* Containers and iterators
    Нужно научиться сделать реалокацию, но не создавать новые объекты
*/

template <typename T>
class vector {
    T* arr_;
    size_t sz_;
    size_t cap_;

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

struct S {
    int x;
    S(int x) : x(x) {};
};

template<>
class vector<bool> {
    char* arr;
    size_t sz_;
    size_t cap_;

    struct BitReference {
        char* cell;
        uint8_t index;

        BitReference(char* cell, uint8_t index)
            : cell(cell), index(index) {}

        void operator=(bool b) {
            if (b) {
                *cell |= (1 << index);
            } else {
                *cell &= ~(1 << index);
            }
        }

        operator bool() const {
            return *cell & (1 << index);
        }
    };
public:

    BitReference operator[](size_t index) {
        return BitReference(arr + index / 8, index % 8);
    }
};

template <typename T>
class Debug {
    Debug() = delete;
};

int main() {
    {
        std::vector<S> v;
    }
    {
        std::vector<bool> v(10);
        v[5] = true;                // это rvalue, которому что-то присваивается
        // Debug d(v[5]);
    }
}
