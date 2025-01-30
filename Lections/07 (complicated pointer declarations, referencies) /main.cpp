#include <iostream>

/*
    *Complicated pointer declarations and Referencies

    *Referencies
    void f(int& a) { a++; }
    takes exactly that object, which you provided

    У нас в плюсах нет коллектора мусора, поэтому при =
    создается новй объект, а не сслылка на уже существующий
    Можем создать ссылку на уже существующий так:
    string s = "Hello";
    string& sr = s; - это ссылка на тот же объект
    (если поменяем sr, то поменяется и s)
    ну в том смысле, что sr на самом деле и есть s
    Адрес s == Адре sr
    *Единственная разница - в области видимости
    (Вызове деструктора и тд)
    ! Ссылка должна быть обязательно проинициализирована при создании
    *Ссылку нужно обязательно проинициализировать lvalue

    *Строго говоря, при операции ++x - возвращаемый тип, это ссылка на x, а не х
    *Также при разыменовании указателя *p - возвращаемый тип это ссылка на объект под указателем

    ! Нельзя создавать ссылку на ссылку
*/

void f(int& y) { ++y; }
void f(int x) { ++x; }

void swap(int& x, int& y) {
    int t = x;
    x = y;
    y = t;
}

int& g(int& x) {
    return ++x;
}

int& f1(int& x) {
    int y = ++x;
    return y;
}

int& f2() {
    int* p = new int(1);
    return *p;
}

int main() {
    {   //* C-style function pointers
        int* a[10]; // массив из 10 указателей на инт
        int (*b)[10]; // указатель на массив из 10

        void (*pf)(int); // указатель на функцию
        // pointer to function which takes int, returns void

        void (*pfa[10])(int); // массив из 10 указателей на функции
        // array of 10 pointers to functions which takes int, returns void

        void (*(*ppf[10])(int))(int);
        // pointer to function, which takes int
        // and returns pointer to function

        int (*(*foo)())();
        // pointer to function which takes nth
        // and returns pointer to function which takes nth and returns int
    }
    {   //! Function calling with referencies
        int x = 5;
        int& y = x;
        // f(x); //! Error but while calling function
        // is is said, that there are two defining of f and they are different
        // as they have different types of arguments
        // but while compiling and calling function
        // compiler can't choose which defining is more priority
        //! So error: call to 'f' is ambiguous
        // f(y); // Same situation
    }
    {
        int x = 5;
        g(x) = 10; // вот такие фокусы можно делать, потому что ссылка возвращаемая то lvalue
        std::cout << x << std::endl; // 10 - поменяли ссылку, что теперь она равна 10
        int t = g(x); // вот это уже новый инт и копия х
        // lvalue to rvalue conversion
        // вот считать значение ячейки это rvalue
        // а взять саму ячейку это lvalue
        // int* p = *p2; // вот тоже конвертация происходит
        // слева l, а справа r
    }
    {   // Ссылка, которая живет дольше объекта
        // Битая ссылка
        int x = 0;
        int& y = f1(x);
        // f1(x) означает ссылку на объеект, который был, пока f1 выполнялась
        // после завершения этот объект уже не существует
        // но ссылка осталась
        // поэтому UB
        //* RE: warning: reference to stack memory associated with local variable 'y' returned
    }
    {   // нет UB, но странно, но так можно
        int& x = f2();
        delete &x;
        //! вот так сделать нельзя! это segfault
        // int y = f2();
        // delete &y;
    }
    {
        int x = 0;
        int* p = &x;
        int*& p2 = p;
        p = new int(1);
        delete p2;
        // same objects p and p2
        // нет типа указателя на ссылки - это ошибка компиляции
    }
    {
        int a[10];
        int(&b)[10] = a; // ссылка на массив из 10
        // вот массив из ссылок создать нельзя
    }
    {
        // можно создавать ссылки на функции
        void (&f2)() = f2;
        // int& f(int) - вот это просто объявление функции, которая возвращает ссылку
        // int& (&f)(int) - а вот это уже ссылка на функцию, которая принимает инт и возвращает ссылку
        int& (&f1)(int&) = f1;
        std::cout << f1 << " " << f2 << std::endl;
    }

}