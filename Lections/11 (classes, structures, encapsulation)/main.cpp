#include <iostream>

/*
    * Basics of OOP
    * Classes and structures, encapsulation

    Будем объявлять свои типы со своими методами (операциями)
    Поля - данные, которые хранятся внутри класса
    Методы - функции, которые описывают поведение класса

    Размер струтуры - сумма размеров всех полей -+ размер паддинга
    Компилятор берет и кладет в память кратным 8
    Вот double нужна память кратная 8, поэтому 16
    Также long long
    А если три инта, то достаточно 12, поскольку только кратные 4

    Поля структуры компилятор не меняет в памяти
    Идут по порядку в памяти

    Также можно доопределять класс или структуру функцияи извне
    Главное писать qualified id S::foo()
    Главное их объявить

    * this / other
    Можно получать объект, который вызвал метод

    Ну и оператор ->
    (*this).x ~ this->x

    * Access modifiers
        * Private
        * Public
        * Protected

*/

class C {
    int x = 10; // private
};

struct S {
    int x = 10;
    double d = 1.1;

    void foo();
    int bar() {
        return this->x;
    }

    // Inner class
    struct {
        char c;
    } a; // Вообще типа что - так можно чтоли
};

void S::foo() {
    std::cout << "foo" << std::endl;
}

int main() {
    {
        S s; // s типа S
        std::cout << s.x << std::endl; // UB непроиниц
        std::cout << sizeof(s) << std::endl;
        s.x = 2;
    }
    {
        S s{2, 3.4}; // Agregate initialize
        // Вот если большое, то CE
        // Не работает с приватными и с конструкторами
        std::cout << s.x << " " << s.d << std::endl;
        s.foo();
        std::cout << s.bar() << std::endl;
    }
    {   // Например так да
        S s;
        s.a.c = 'a';
        std::cout << s.a.c << std::endl;
    }
    {   // Local class
        struct C {
            int x;
        };
        C c;
    }
    {   // access error
        C c;
        // std::cout << c.x << std::endl; // error: 'x' is a private member of 'C'
        std::cout << (int&)c << std::endl;
        // грязные игры, потому что поле то вообще-то приватное
    }
}