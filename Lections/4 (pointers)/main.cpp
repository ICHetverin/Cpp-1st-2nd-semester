#include <iostream>

/*
    *Указатели
    &x - унарный оператор нахождения индексации памяти
    16 cc - индексация памяти
    int* - указатель на int
    * - унарный оператор разыменования (lvalue)
    & - унарный оператор адреса (rvalue)
    требуется, чтобы аргумент амперсанта был lvalue

    Размер - implementation-defined (но кажется 8 байт)
    *nullptr - нулевой указатель
*/

int main() {
    {
        int x = 1, y = 0;
        std::cout << &x << " " << &y << std::endl;
        int* p = &x;
        std::cout << p << " " << (p - 1) << std::endl;
    }
    {
        std::vector<int> v = {1, 2, 3, 4, 5};
        int* p = &v[0];
        std::cout << *++p << std::endl; // 2 элемент - 2
        *p = 25;
        std::cout << v[1] << std::endl; // 2 элемент - 25
    }
    {
        int x = 1;
        int*p = &x;
        std::cout << &*p << " " << *&p << std::endl; // the same
    }
    int* p;
    {
        int b = 5;
        p = &b;
        std::cout << p << std::endl;
    }
    std::cout << p << " " << *p << std::endl; //* UB, BUT "memory reuse" b's lifetime ended, but pointer
    //* is still alive
    {
        int x = 0;
        double d = 3.14;
        // std::cout << (&x < &d) << std::endl; //!error: comparison of distinct pointer types ('int *' and 'double *')
    }
    {
        int y = 20;
        void* p = &y; // void pointer can cast to any pointer
        std::cout << p << std::endl;
        // No operations with void*
    }
}