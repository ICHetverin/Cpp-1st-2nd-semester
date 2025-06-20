#include <iostream>

/*
    * Memory Layout due to inheritance
    * Constructors and destructors

    Когда наследуемся от чего-то это что-то становится частью вас
    Все поля то сохраняются

    * EBO Empty Base Optimization
        Если просто пустой класс, то по памяти = 1
        А если наследуется пустой класс - то разрешается
        ничего не занимать в памяти

    * Constructors
        Когда создается объект при наследовании, то сначала вызывается
        конструктор родительского класса, а уже затем конструктор нынешнего

    * Приведение типов при наследовании

*/

struct Base {
    int x; // V1
    // void f() {}; // V2
    Base() = default;
    Base(int x) : x(x) {}
    Base(const Base& other) : x(other.x) {}
};
// 4b

struct Derived : Base {
    double y;
    Derived(int y) : y(y) {}
    // Нет дефолтного конструктора у Base
    // И поэтому можно делать так
    Derived(int y) : Base(0), y(y) {}
    using Base::Base;
    /*
        Что получается
        у Base есть свой конструктор копирования
        а у Derived нет
        Поэтому в ситуации Derived d2 = d, a d = 1
        y должен был бы стать 0
        Поскольку при иницализации он 0 ну и вот это все
        * Но получается, что компилятор создает неявный конструктор копирования
        для Derived, который приоритетнее конструктора Base
    */
};
// 16b

struct Empty {};

struct Base1 {
    int x = 1;
};

struct Derived1 : Base1 {
    int y = 2;
};

void f(Base1& b) {
    std::cout << b.x << std::endl;
}

// Slicing Срез при копировании
// void f(Base1 b) {
//     std::cout << b.x << std::endl;
// }
// При такой штуке полноценно скопируется объект Base
// Будет неявно сегенерирован конструктор Base1 от Derived1
// ААААА - в Derived уже сущетсвует Base
// Просто при вызове функии он как и обычно скопируется полностью

int main() {
    {   // V1
        std::cout << sizeof(Derived) << std::endl; // 16
        // Поля лежат в порядке объявления
        // V2
        std::cout << sizeof(Derived) << std::endl; // 8
        std::cout << sizeof(Empty) << std::endl; // 1
    }
    {
        Derived d = 1;
        Base b = 1;
        // Derived d2 = b; // CE потому что конструктор копирования не наследуется!
    }
    {   //* Cast due to inheritance
        Derived1 d;
        f(d); // It's okay cause of d - children
        //* Вся идея наследования в этом!
        // Неявный каст просто
        // Аналогия такая же, что и с кастом констов
        // Можно передавать и нынешнего и дочернего
        // 1) По ссылке
        // 2) Копированием объекта
        // 3) По указателю
    }
}