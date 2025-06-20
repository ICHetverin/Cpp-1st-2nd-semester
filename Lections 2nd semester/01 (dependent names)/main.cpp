#include <iostream>
#include <array>

/*
* Dependent names
    Задана struct S, у которой есть шаблонное определение
    при S<T>::A* непонятно, что имеется ввиду
    это decl или expr (в зависимости от T)
    ! А - dependent name
    В зависимости от Т это декл или экспр

    Во всех таких ситуациях парсер считает это expression

    Это происходит потому что компилятор сначала парсит написанное,
    не делая никаких шаблонных подстановок

* Наследование при шаблонном классе
    Пусть есть какой-то шаблонный заданный класс base, и наследованный
    от него Derived. Так вот компилятор не может пойти и посмотреть код тела
    класс родителя. Поэтому он не знает поля шаблонного родителя!

    Если хотим обратиться к полю шаблонного родителя, то нужно
    this-> ... или Base<T> ...

* Two-phase translation
    Компилятор в каком-то смысле проходит по коду и парсит его 2 раза
    Сначала как просто осмысленные конструкции
    Затем при подстановке шаблонных параметров
*/

template <typename T>
struct S {
    using A = int;
};

template <>
struct S<double> {
    static const int A = 5;
};

template <typename T>
void f() {
    S<T>::A* x;
    // typename S<T>::A* x; // - однозначно будет declaration
}

// ---------

// int x = 0;

template <typename T>
struct S {
    template <typename N>
    using A = std::array<int, N>;
};

template <typename T>
void f() {
    typename S<T>::A< 10 > x;
    // могло распарситься как expr А < 10 > x без typename
    // слово typename еще не считает, что это тип шаблона, чтобы был тип
    // именно шаблона нужно:
    //* typename S<T>::template A<10> x;
}

// ---------

template <typename T>
struct Base {
    int x = 0;
};

template <>
struct Base<double> {};

template <typename T>
struct Derived : Base<T> {
    void f() { ++x; }
    // починить можно так
    void f() { ++this->x; }
};

int main() {
    {
        f<int>();
        // f<double>();
        // ! будет ошибка, нельзя чтобы в зависимости от Т
        // менялся смысл decl or expr
    }
    {

    }
}