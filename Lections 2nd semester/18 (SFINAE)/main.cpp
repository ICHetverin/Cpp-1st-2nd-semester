#include <iostream>
#include <vector>

/*
* Metafunction, SFINAE, concepts and constraints

* Basic primitives for template metaprogramming

* SFINAE (Substitution failure is not an error)
    если в момент объявления функции (подстановки T в функцию)
    при неподходящем Т компилятор просто откинет ее
*/

template <typename T>
struct type_identity {
    using type = T;
};

template <typename T>
using type_identity_t = type_identity<T>::type;

template <typename T, T x>
struct integral_constant {
    static constexpr T value = x;
};

template <bool b>
using bool_constant = integral_constant<bool, b>;
using true_type = bool_constant<true>;
using false_type = bool_constant<false>;

template <typename T>
struct is_lvalue_reference: false_type {};

template <typename T>
struct is_lvalue_reference<T&>: true_type {};

template <typename T>
using is_lvalue_reference_v = is_lvalue_reference<T>::value;

template <typename... Types>
struct conjuction {
    static constexpr bool value = (Types::value && ...);
};
//----------------------------------------------
template <typename T>
auto T(f) -> typename T::value_type {
    std::cout << 1;
};

// C-style variadic function
void f(...) {
    std::cout << 2;
}

int main() {
    {
        f(0);
    }
}