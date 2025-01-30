#include <iostream>

/*
    * Non-obvious problems with virtual functions

    Виртуальные функции вообще нельзя оставлять без определния, только если это не pure

    * Из конструктора / поля(когда иниц функцией) вызов виртуальной функции работает не по правилам виртуальной функции
    * а как вызов обычной функции
*/

struct Base1 {
    int x;
    // static virtual void f() {} //! NO NO NO CE
    virtual void f() {}
};

struct Derived1 : Base1 {};

//* V1 and V2
// struct Base {
//     virtual void h() = 0;
//     void f() {
//         std::cout << "f" << std::endl;
//         h();
//     }
//     Base() {
//         std::cout << "Base ";
//         // h(); // V1
//         f(); // V2
//     }
//     virtual ~Base() = default;
// };

// struct Derived : Base {
//     int x;
//     void g() {
//         f();
//     }
//     void h() override {
//         std::cout << "h " << x;
//     }
//     Derived() : x(0) {
//         std::cout << "Derived ";
//     }
// };

//* V3
struct Base {
    virtual void f(int x = 1) {
        std::cout << "Base " << x << std::endl;
    }
};

struct Derived : Base {
    void f(int x = 2) override {
        std::cout << "Derived " << x << std::endl;
    }
};

int main() {
    {   // linker error
        Derived1 d;
    }
    {   //* V1
        // Derived d;
        //! libc++abi: Pure virtual function called!
        //! Base [1]    70485 abort      ./22
        /*
            Что произошло
            1) Вызов конструктора Base
            В нем вызов h
            Но h к тому моменту (в теле конструктора) еще пока pure function
            Поэтому ее вызов работает по правилам вызова обычной функции
            Из-за этого такая ошибка
        */
    }
    {   //* V2
        // Derived d;
        //! libc++abi: Pure virtual function called!
        //! [1]    72305 abort      ./22
        // Ну ровно то же
        // На момент вызова конструктора h - pure function
    }
    {   //* V3
        Derived d;
        Base& b = d;
        b.f(); // Derived 1
    }
}
