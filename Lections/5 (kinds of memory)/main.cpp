#include <iostream>

/*
    *Types of memory
    *- Data (static memory (global, namespace local, ...))
    *- Text
    *- Stack (automatic memory (local variables, ...)) ~ 8Мб
    при новых данных локальных - они доабвляются в стек, при окончании области видимости
    они удаляются со стека (push, pop)
    *- Heap (dynamic memory (new, delete, malloc, calloc, realloc, ...))
    вот она выдается по ходу рантайма

    *-- functions:
    void foo(int y) {
        cout << y + 1;
    }
    на стэк складывается y (данные при функции), а также
    адрес след строчки после функции в теле, где вызывается

    !Stack overflow - весь стэк забивается (seg fault)

    *Static переменные
    "просим" чтобы переменная лежала не в стэковой, а в статической памяти - постоянный адрес
*/

void foo(int x) {
    std::cout << x++ << std::endl;
    foo(x);
}

void foo() {
    int* p = new int;
    std::cout << p << " " << *p << std::endl;
    // delete p;
}

int main() {
    // foo(0); // - stack overflow (seg fault)
    {
        int* p = new int; // выделение памяти для int на 4 мб
        *p = 1000;
        int a = 100;
        std::cout << &a << " " << p << std::endl; // в совсем разных местах лежат
        delete p; // без delete он будет лежать дальше на стэке
        // delete p; //! Abort - second time free the same object
        // delete ++p; //! Abort - space for this object was not allocated
    }
    // std::cout << p << std::endl; // без delete будет memory leak (утечка памяти)
    {
        // while (1) {
        //     foo();
        // }
        // memory leak в чистом виде - занимаем память и не овобождаем ее
    }
    {
        int* p = new int(15);
        int* pp = new int(20);
        delete p, pp; // deleting only the first item WARNING in compiler
        delete (p, pp); //deleting only the last item WARNING in compiler
    }
    {
        static int x = 0; // local static variable -- Compile Time
    }
}
