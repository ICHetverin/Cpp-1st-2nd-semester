#include <iostream>

/*
    * Inheritance
    * Public, Private and Protected

    Области видимости все включают, а вот доступность это уже другое

    * Private inheritance
    Только я и мои друзья знают о том, что я наследник
    Для всех извне Derived родительская часть недоступна

    * Protected inheritance
    Только лишь я сам, мои друзья и мои наследник имеют доступ к
    родительской части, а для всех извне - нет

    * Понятия видимости и доступности
    Если у родительского и дочернеего будут одинаковые имена функций - про это

    * Если равные имена - то всегда от наследника
*/

struct Base1 {
protected:
    int x;
public:
    void f() {};
};

// struct Derived : public Base { // V1
struct Derived1 : private Base1 { // V2
    int y;
    void g() {
        std::cout << x << std::endl;
    };
};

struct Granny {
    friend int main(); // add a little spice
    int x;
    void f() {};
};

struct Mom : protected Granny {
    int y;
    void g() {
        std::cout << x << std::endl;
    }
};

struct Son : Mom {
    int z;
    void h() {
        std::cout << x << std::endl;
        // Так можно для всех дочерних (когда Mom : [protected / public] Grannny)
    }
};

struct Base {
    void f(int) {
        std::cout << 1 << std::endl;
    }
};

struct Derived : Base {
    void f(double) {
        std::cout << 2 << std::endl;
    }
};

int main() {
    {   // V1 - OK
        // Derived d;
        // std::cout << d.x << std::endl; // UB
    }
    {
        // V2 - Not Ok
        // Derived d;
    }
    {   // 1 - Mom : protected Granny
        Son s;
        s.z;
        s.y;
        // s.x; // CE - извне Mom
    }
    {   // 2 - Granny { friend int main(); }
        Son s;
        s.z;
        s.y;
        // s.x; // СЕ - ограничение накладывает Mom при наследовании, а не Грэнни
        // Строгая Мама запрещает общаться доброй Бабушке с внуком
    }
    {   //* Visibility
        Derived d;
        d.f(0); // Конверсия в double, потому что дочерний приоритетнее
        // Тут нет перегрузки, поэтому нет СЕ
        // Просто затмение имен (родительская сменилась дочерней)
    }
}