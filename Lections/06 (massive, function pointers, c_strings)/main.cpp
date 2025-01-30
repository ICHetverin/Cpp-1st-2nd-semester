#include <iostream>
#include <algorithm>

/*
    *Massives and Pointers and C strings and Pointers to functions

    *Дефолтные аргументы
    Ставятся всегда конце!

    *Variadic functions
    *void f(const int x, ...)
    Но никогда почти ПОЧТИ не используем
    И по код-стайлу не пользуемся
    Юзаем там внутри арги
*/

void foo(int* a) {
    std::cout << 2[a] << std::endl;
}

// void foo(int a[3]) { //! Redefinition of foo (pointer ~ massive[i])
//     std::cout << "Boom!\n";
//     std::cout << sizeof(a) << std::endl; // 8
// }

void f(int** a) {}; // 1
void f(int (*a)[5]) {}; // 2
// void f(int* a[5]) {}; // 3 //! Redefenition of first

//* Comparator
bool cmp(int x, int y) {
    return x > y;
}

void f1(int) {};
void f1(double) {};
void f2(long long);

int main() {
    {
        int a[5]{1, 2, 3, 4, 5};
        std::cout << a[1] << std::endl; // 1
        // std::cout << a[5] << std::endl; // затер память UB
    }
    {
        // int a[100'000'000]{1}; // UB - seg fault (stack overflow)
        static int a[100'000'000]{1}; // no seg fault - память будет запрошена в части Data
        // а не в стэке
    }
    {
        int a[5]{1, 2, 3, 4, 5};
        int* p = a;
        std::cout << p[1] << std::endl; // 1
        std::cout << *(a + 3) << std::endl; // Array to pointer conversion ~ 4
        std::cout << p[5] << std::endl; // UB
    }
    {
        // a[2] === *(a + 2)
        int a[5]{1, 2, 3, 4, 5};
        int* p = a + 3; // 4
        std::cout << *p << std::endl; // 4
        std::cout << p[-1] << std::endl; // 3
        std::cout << 2[a] << std::endl; // 3
    }
    {
        // Differencies between pointer and massive
        int a[5]{1, 2, 3, 4, 5};
        int b[5]{};
        // a = b; // Compile error
        // ++a; // Compile error
        int* p = a;
        std::cout << sizeof(p) << " " << sizeof(a) << std::endl;
    }
    {
        // Massive to pointer conversion
        int a[5]{1, 2, 3, 4, 5};
        foo(a);
    }
    {
        int* a = new int[100]; //! Pointer - not massive
        std::cout << *(a - 2) << std::endl; // UB try to check what memory reserved
        delete[] a; //! [] for massives(pointers)
        // Квадратные скобочки потому, что когда мы инициализируем массив new int[100]
        // то это число элементов - 100 где-то сохраняется и когда мы удаляем,
        // то он поднимает это число и решает что делать дальше
    }
    {
    //     // UB how to break free process //! Abort
    //     std::vector<int> a(100);
    //     a[-1] = 500000000;
    //     delete[] &a[0]; //! Abort double free
    }
    {   // Massives with variable length
    //* ISO C++ forbids VLA (variable length array)
        // int n;
        // std::cin >> n;
        // int a[n];
        // std::cout << a[1] << std::endl;
    }

    // 2 dimension arrays

    {
        int a[5][5];
        int* b[5]; // Array of 5 pointers to int
        int (*c)[5]; // Pointer to array of 5 ints
    }

    //* C strings
    {
        char s[] = "Hello";
        const char* str = "Hello!"; //* ISO forbids conversion from string literal to char*
        std::cout << s << std::endl; // Определенные правила и поэтому выводится вся строка до \0
    }
    {   //! Bus Error
        // char* s = "Hello";
        // s[4] = 'a';
        // std::cout << s << std::endl;
    }
    {   //* Pointers to functions
        int a[5]{1, 9, 3, 6, 10};
        std::sort(a, a + 5);
        for (int i = 0; i < 5; ++i) {
            std::cout << a[i] << " ";
        }
        std::cout << std::endl;
        std::sort(a, a + 5, &cmp); //* using comparator
        // std::sort(a, a + 5, cmp); //Неявная конвертация function -> pointer to function
        for (int i = 0; i < 5; ++i) {
            std::cout << a[i] << " ";
        }
        std::cout << "\n";

        bool (*p)(int, int); //* Указатель на функцию, которая принимает два инта и отдает бул
        std::cout << (void*)p << std::endl;
    }
    {
        void (*p)(int) = &f1;
        void (*p2)(double) = &f1;
        std::cout << (void*)p << " " << (void*)p2 << std::endl; // 2 different addresses
        //! Нельзя брать указатель на функцию, которая не определена!!
        //! Это фактически значит взять место в машинном коде, где пустота
        //! Ошибка линковщика
        // void (*p3)(long long) = &f2;
        // std::cout << (void*)p3 << std::endl;
        // error: linker command failed with exit code 1 (use -v to see invocation)
    }
}