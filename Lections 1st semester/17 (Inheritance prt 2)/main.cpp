#include <iostream>

/*
    * Inheritance and Visibility part 2

    * Приватность идет уже после перегузки и выбора в области видимости

    1) берем кандидатов на перегрузку из области видимости
    2) выбор перегрузки
    3) определение приватности

    Короче можно делать какие-то методы родителя приватные доступными для себя
    и наоборот (публичные родителя -> приватные для себя)
    с помощью uisng Parent
    и грамотно расставить private / public
    private:
        using Base::f;
    public: ...
    но при public насследовании можно будет вызвать фцию через Base::f() например

    * Все наследуется, просто может не быть доступа, но они все видны!

    В классе можно делать только declaration, expression просто нельзя
    - они и не распарсятся

*/

struct Base {
protected:
// private:
// public:
    // void f(int) { // V12
    void f(double) { // V3
        std::cout << 1;
    }
};

struct Derived : Base {
// private: // V1
    using Base::f; // V2
    // in V2 cant use cause access denied
    // int f(int) { // V1
    void f(int) { // V2
        std::cout << 2;
        // return 0;
    }
};

struct Granny {
    int x;
    void f() {};
};

struct Mom : private Granny {
    friend int main();
    int x;
};

struct Son : Mom {
    int x;
    // void f(Granny& g) { // само имя приватное, поэтому даже по ссылке принять его нельзя
    void f(::Granny &g) { // вот так уже корректно (тип Granny именно внешне - ссылка)
        std::cout << g.x << std::endl;
    }
};

int main() {
    {   // V1
        // Derived d;
        // d.Base::f(0);
    }
    {   // V2
        // Derived d;
        // d.f(0); // Та которая внтури тела объекта - главнее
    }
    {   // V3
        Derived d;
        d.f(2.0);
        d.f(0);
        // Вот уже вызываемся от Base
    }
    {
        Son s;
        s.Mom::Granny::x;
        s.Granny::x; // то же и можно также
    }

}
