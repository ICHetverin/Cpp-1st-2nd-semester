#include <iostream>
#include <cstdlib>
#include <exception>

/*
* Exceptions
    Чисто RT концепция
    из стандартных операторов у
    malloc, dynamic_cast, throw

* RT Errors (how it differs from exceptions)
* Иерархия стандартных исключений
    std::exception
    (constructor)
    (destructor)
    operator=
    what
    ! logic error (виноват пользователь)
    ! runtime error (не виноват)

*/

int divide(int a, int b) {
    if (b == 0) {
        throw std::logic_error("Divide by zero!");
    }
    return a / b;
}

int main() {
    {
        void* p = malloc(1234567);
    }
    {
        try {
            divide(1, 0);
        } catch (std::logic_error& err) {
            std::cout << err.what();
        }
    }
    {
        // divide(1, 0);
    }
    {
        try {
            new int[400'000'000'000'000];
        } catch (...) {}
    }
    {
        std::vector<int> v;
        int x;
        std::cin >> x;
        try {
            v[1000000] = 1;
            // std::cout << 5 / x;
        } catch (...) {
            std::cout << "Caught!";
        }
        //! seg fault - не exception
    }
}