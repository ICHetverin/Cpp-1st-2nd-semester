#include <iostream>

/*
    * Const Static and explicit methods

    * mutable - запрещает делать поле константным
    даже при объявлении объекта константмым
    - например, сколько раз что-то вызвали в объекте или подсчитываем что-то
*/

struct S {
    char* arr;
    mutable int count = 0;
    void f() const {
        count++;
        std::cout << 1 << std::endl;
    }
    void f() {
        std::cout << 2 << std::endl;
    }

    static void h() {
        std::cout << "hi" << std::endl;
    }

    // static int x = 15; // error
    static int x;

    char& operator[](size_t index) {
        return arr[index];
    }
    const char& operator[](size_t index) const {
        return arr[index];
    }
};

int S::x = 2; // относятся ко всему классу -> ко всем объектам класса сразу
// например сс
// или Синглтон - объект класса который подразумевается только один на все время программы

struct Singletone {
private:
    Singletone() {};
    static Singletone* ptr;

    Singletone(const Singletone&) = delete;
    Singletone& operator=(const Singletone&) = delete;
public:
    static Singletone& get() {
        if (ptr == nullptr) {
            ptr = new Singletone();
        }
        return *ptr;
    }
};

Singletone* Singletone::ptr = nullptr;

// Строгая типизация!
struct Latitude {
    double value;
    explicit Latitude(double value) : value(value) {}
    // запрещает неявно вызывать конструктор!

    //* оператор приведения типа
    // теперь только через явный каст
    explicit operator double() const {
        return value;
    }
}; // широта

struct Longitude {
    double value;
    explicit Longitude(double value) : value(value) {}
}; // долгота

// допсутим хочу в функцию в определенном порядке принимать (широта, долгота)
// но они ведь одного типа
// и чтобы не перепутать можно сделать две разных структуры

class BigInteger {};

BigInteger operator""_bi(unsigned long long value) {
    return BigInteger();
}

int main() {
    {
        // const S s;
        // s.f();
        //! error: 'this' argument to member function 'f' has type 'const S', but function is not marked const
        // и нужно явно указывать что методы const это для const объектов
        // так работает, потому что по факту первым (0) аргументов у f идет ссылка на объект S
        // и там будет неявный каст от конст к неконст -> ошибка
        // у константных методов нельзя менять поля
        //* можно делать перегрузку по константности функции
        //* по сути констатность самого объекта this
    }
    {
        // const S s;
        // s.f();
        // s.f();
        // s.f();
        // std::cout << s.count << std::endl;
    }
    {
        //* Static methods
        // you can use them without creating an object
        // ну то есть они не относятся к какому либо объекту, а относятся ко всему класс в целом
        S::h();
        std::cout << S::x << std::endl;
    }
    {
        // Singletone s; //! Wrong
        // Singletone.get(); //! Wrong
        Singletone& s = Singletone::get(); //* Correct
    }
    {
        BigInteger bi = 1_bi;
        // корректно, потому что можно самому определять такие методы для своего класса
    }
}