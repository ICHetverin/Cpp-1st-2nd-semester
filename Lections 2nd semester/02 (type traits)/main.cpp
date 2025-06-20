#include <iostream>
#include <type_traits>

/*
* Metafunctions and type traits
    Метафункции возвращают значение от принимаемого типа
    например сравнение типов

    ! typeid(...) - плохо, потому что это RT проверка
    и нужна как бы для полиморфных объектов, которые не известны
    в CT

    * if constexpr (is_same<T, U>::value) {x = y;}
    вот это уже в CT
    это важно, потому что дальше код также компилируется
    constexpr просто опускает почти всю дальше семантику, если условие не True

* Примеры метафункций (type_traits)
    remove_reference
    remove_const
    is_const
    is_reference

    : std::true_type ~ static const bool value = true
    : std::false_type ~ static const bool value = false

    * rank - длина массива (что будет, если снять [] + 1)
    * decay - снимает с типа все украшения

* Vatiadic templates
    шаблоны с переменным кол-вом аргументов

    template <typename... Types>
    f(Types... tx) - распаковка параметров в tx

* Sizeof... (пакет)
    возвращает размер пакета в CT
*/

template <typename T, typename U>
struct is_same {
    static constexpr bool value = false;
};

template <typename T>
struct is_same<T, T> {
    static constexpr bool value = true;
};

// стандарт в c++ 17
template <typename T, typename U>
const bool is_same_v = is_same<T, U>::value;
// alias на type_traits

template <typename T, typename U>
void f(const T& x, const U& y) {
    //...
    if (typeid(x) == typeid(y)) {}
    //...
    if (is_same<T, U>::value) {x = y;}
    // это вычисляется еще пока в RT
    if constexpr (is_same<T, U>::value) {x = y;}
    // вот это уже в CT
    // это важно, потому что дальше код также компилируется
}

// ------
// Metafunctions (type traits)

template <typename T>
struct remove_reference {
    using type = T;
};

template <typename T>
struct remove_reference<T&> {
    using type = T;
};

template <typename T>
void f() {
    typename remove_reference<T>::type x;
};

// ------
// Тернарный оператор для типов!
// bool - всегда constexpr

template <bool B, typename T, typename F>
struct conditional {
    using type = F;
};

template <bool B, typename T, typename F>
struct conditional<true, T, F> {
    using type = T;
};

template <bool B, typename T, typename F>
using conditional_t = typename conditional<B, T, F>::type;
// шаблонный alias - стандарт

// -------
//* Variadic templates

template <typename ... Types>
void f(Types... tx) {
    // g(tx); // например
}
// tx - пачка типов

template <typename Head, typename... Tail>
void print(const Head& head, const Tail&... tail) {
    std::cout << head << std::endl;
    print(tail...);
}

void print() {} // когда аргументов не осталось

// --------
//* Метафункция is_homogeneous (все типы в пакете одинаковые)

template <typename First, typename Second, typename... Types>
struct is_homogeneous {
    static constexpr bool value = std::is_same_v<First, Second>
            && is_homogeneous<Second, Types...>::value;
};

template <typename First, typename Second>
struct is_homogeneous<First, Second> {
    static constexpr bool value = std::is_same_v<First, Second>;
};

int main() {
    {

    }
}