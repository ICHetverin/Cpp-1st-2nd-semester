#include <iostream>

/*
* Fold expressionsn (since c++ 17)
    Fold expression - expression, когда берется сначала пакет, а затем
    бинарным оператором соединяется с многоточием

    Syntax:
    (pack op ...) - u (право-ассоциативный)
    (... op pack) - u (лево-ассоциативный)
    (pack op ... op init) - b (право-ассоциативный)
    (init op ... op pack) - b (лево-ассоциативный)

* Non-obvious template using
    * CRTP (Curiously Recursive Template Pattern)
    * Expression templates
    (пример с суммой векторов из википедии)
*/

template <typename... Types>
struct all_pointers {
    static const bool value = (std::is_pointer_v<Types> && ...);
};

template<typename Head, typename... Tail>
struct is_homogeneous {
    static const bool value = (std::is_same_v<Head, Tail> && ...);
};

// пробуем релизовать print c помощью этой темки

template <typename... Types>
void print(const Types&... types) {
    (std::cout << ... << types); // (лево-ассоциативный)
};

// ------
//* CRTP

template <class T>
struct Base {
    void interface() {
        // ...
        static_cast<T*>(this)->implementation();
        // ...
    }

    static void static_func {
        // ...
        T::static_sub_func();
        // ...
    }
};

struct Derived : Base<Derived> {
    void implementation();
    static void static_sub_func();
};

// ------
//* Expresion templates
// реализуем класс вектор (мат. вектор)
// будем учиться складывать вектора
// будем получать сумму или произведение части ответа
// без вычисления всей суммы или произведения

int main() {};