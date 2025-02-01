#include <iostream>
#include <cstring>

/*
    * Friend Classes and Destrcutor / Constructor
    Такие функции или классы, которые имеют доступ к полям

    * implicitly develop Default Constructor
    например у нас есть ни одного своего конструктора, тогда он будет определен неявно компилятором
*/

class C {
private:
    int x = 5;
public:
    void f (int y) {
        std::cout << x + y << std::endl;
    }

    friend void g(C, int); // Вот так можно объявлять в любом месте friend фцию/класс
    // Но если определить и здесь и вне - то это будет redefinition
    // Потому что это объявление функции в глобал
};

void g(C c, int y) {
    std::cout << c.x + y + 1 << std::endl;
}

class K {
private:
    void f(int) {
        std::cout << 1 << std::endl;
    }
public:
    void f(float) {
        std::cout << 2 << std::endl;
    }
};

class Complex {
    double re = 0.0;
    double im = 0.0;

public:
    Complex(double re) : re(re) {}
    // Если внутри поля, то поле просто проиницализр 0 до этого и потом приравняется к значению
    // Вот у нетривиального поля - делаем сначала иниц 0, а потом присваивание (2 операции)
    // Конечно лучше 1 операцию
    Complex(double re, double im) : re(re), im(im) {}
    // Complex(double re, double im) : im(im), re(re) {} // Ну вот например это уже warningи так делать не нужно
};

class String {
    char* arr = nullptr;
    size_t size = 0;
    size_t capacity = 0;

public:
    // String() {} // Default constructor
    String() = default;
    // = defalut лучше чем самому писать простой
    // explicitly declare implicitly defined
    // явно хотим указать, чтобы компилятор создал неявный конструктор
    String(size_t n, char c) : arr(new char[n+1]), size(n), capacity(n + 1) {
        memset(arr, c, n); // Вот первый аргумент void* потому что без разницы какой тип
        // std::fill(arr, arr + size, c); // Вот ровно то же самое
    } // Non-trivial constructor
    String(std::initializer_list<char> list) //  TODO: как под капотом initializer list
            : arr(new char[list.size() + 1])
            , size(list.size())
            , capacity(list.size() + 1) {
        std::copy(list.begin(), list.end(), arr);
        arr[size] = '\0';
    } // Initializer list constructor

};

int main() {
    {
        C c;
        g(c, 5);
    }
    {   //* Жесткий пример
        K k;
        // k.f(3.14);
        // Что вызывается?
        // 1 или 2?
        // Когда проверили - ответ такой, потому что
        // при линковке сначала проверяется конвертация в типы
        // тут равнозначно (что в int, что в double)
        // поэтому уже возникает ошибка - непонятно кому сопоставить
        // и только потом вызывается проверка приватности / доступа!
        // k.f(2); // ну тут очев что CE
    }
    {
        Complex c(2.5);
        Complex c2 = 3.4;
        Complex c3{7.0};
    }
    {   // * Initializer List
        std::vector<int> v = {1, 2, 3, 4, 5};
        /*
            Вот это например как работает, потому что
            как тут вызывается конструктор, ведь это операция инициализиации
            и тут вызывается именно конструктор!
            Так вот {} означают:
                1) Agregate initialize
                2) Constructor initialize
                3) Constructor with initializer list
            Тут встречается именно 3 тип
        */
    }
    {
        String s1; // Default initializaiton
        String s2 = {2, 'a'}; // Вот это тоже уже от Initializer list
        // Сначала идут от листа (если есть такой конструктор, иначе все остальное)
        String s3 = {1, 2, 3, 4, 4};
        // А вот обычный конструктор от скобок всегда такой
        String s4(2, 'a');
    }
}

