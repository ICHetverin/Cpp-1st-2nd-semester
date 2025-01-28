#include <iostream>
#include <cstring>

/*
    * Operators overloading
    * Operator SpaceShip!
*/

struct Complex {
    double re = 0.0;
    double im = 0.0;

    // Вот такой знак & значит только lvalue
    // Вот такой знак && значит только rvalue
    // Без знака для любых
    Complex* operator=(const Complex& other) & {}

    Complex(double re) : re(re) {}
    Complex(double re, double im) : re(re), im(im) {}

    //* operator+ v1.0
    // Complex operator+(const Complex& other) const {
    //     // Чтобы вызываться от объектов, где левый аргумент был конст числом
    //     return Complex(re + other.re, im + other.im);
    // }

    // Короче, чтобы операция с таким типом могла быть симметричной
    // с + 3.14 = 3.14 + с
    // Нужно определять бинарные операции вне класса
    // Не как методы внутри, а только объявить

    //* operator+= v1.0
    // Complex& operator+=(const Complex& other) {
    //     *this = *this + other;
    //     return *this;
    // }
    // Очень долго (полное копирование двух объектов и их сложение)

    //* operator+= v1.1
    Complex& operator+=(const Complex& other) {
        re += other.re;
        im += other.im;
        return *this;
    }

    // 3-way comparison
    // SpaceShip operator since C++20
    // All standart types uses it
    // Strong ordering: a = b -> f(a) = f(b)
    // Weak ordering: a = b !-> f(a) = f(b) (не всегда следует)
    // И вот этот оператор будет действовать также для левого аргумента
    std::weak_ordering operator<=>(const Complex& other) const = default;
    // Если написать свой spaceship, то операторов == и != не будет
    // Потому что доопределяться они могут неэффективно
    // И == можно сделать проще, иначе он всегда бы был за линейное время
};

//* operator+ v1.1
// Complex operator+(const Complex& left, const Complex& right) {
//     return Complex(left.re + right.re, left.im + right.im);
// }

//* operator+ v1.2
Complex operator+(const Complex& left, const Complex& right) {
    Complex result = left;
    result += right;
    return result;
}

//* String stream operators
// std::ostream& operator<<(std::ostream& out, const String& str);
// std::istream& operator>>(std::istream& in, String& str);

bool operator<(const Complex& a, const Complex& b) {
    return a.re < b.re || a.re == b.re && a.im < b.im;
}
bool operator>(const Complex& a, const Complex& b) {
    return b < a; //! Запомнить
}
bool operator<=(const Complex& a, const Complex& b) {
    return !(a > b); //* Ну и то же для противоположного
}

struct UserId {
    int value = 0;

    UserId& operator++() {
        ++value;
        return *this;
    }
    UserId operator++(int) {
        UserId copy = *this;
        ++value;
        return copy;
    }
};

// Functor, functional class
struct Greater {
    bool operator()(int x, int y) const {
        return x > y;
    }
};

int main() {
    {
        Complex c(1.0);
        c + 3.14; // correct - c.operator+ double
        3.14 + c; // incorrect - double.operator+ c
    }
    {
        Complex a(1.0);
        Complex b(2.0);
        Complex c(3.0);
        // a + b = c; // ОК, потому что присваивание rvalue плохо только для стандартных типов
        // Фиксится сверзу написано как
    }
    {
        std::vector<int> v(10);
        std::sort(v.begin(), v.end(), Greater());
    }
}
