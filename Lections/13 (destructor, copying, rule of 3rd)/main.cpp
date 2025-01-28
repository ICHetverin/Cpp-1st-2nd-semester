#include <iostream>
#include <cstring>

/*
    * No default Constructor
    В каких случаях нельзя определить дефолтный конструктор, даже если явно его указать
    = default
        - Когда поле класса является таким, что его нельзя инициализровать по дефолту
        Ну например,
        1) Ссылка - нельзя определить по дефолту
        2) Константа - можно определить по дефолту, но зачем тогда она в классе
        * Можно конечно инициализировать из глобала переменными (ссылки, константа)
        Так да - можно

    * Destructor вызывается перед выходом из scope
    Тривиальный деструктор и не пишется

    * Constructo Copy
*/

int x = 0;

struct C {
    int& r = x;
    // const int& k;
    // ссылку нельзя переприсвоить!
    // C() : k(5) {} // Вот это например UB
    // Потому что ну продлять жизнь полям класса так это UB
    // C(int y) {
    //     r = y;
    // }
    // тут просто х меняется, потому что это ПРИСВАИВАНИЕ
    C(int y) : r(y) {}
    // Еще CE, потому что уже был указан конструктор с такими переменными
    // вот это уже совсем другое
    // тут удет висячая ссылка на y
};

class String {
    char* arr;
    size_t size;
    size_t capacity;
    // Делегирующий конструктор, который делает общее для многих
    String(size_t n) : arr(new char[n + 1]), size(n), capacity(n + 1) {
        arr[size] = '/0';
    }
public:
    String() = delete; // Вот это запретить конструктор от определенных аргументов
    String(size_t n, char c)
            : arr(new char[n + 1])
            , size(n)
            , capacity(n + 1) {
        memset(arr, c, size);
    }
    // И можно допустим с делегирующим конструктором уже так:
    /*
        String(size_t n, char c) : String(n) {
            memset(arr, c, size);
        }
    */

    // COW string
    // Короче пока [] = х не применяют к строчке, то можно использоватьее как ссылку
    // Но вот когда применяют - тогда и нужно полноценно копировать
    String(const String& other): arr(new char[other.capacity]), size(other.size), capacity(other.capacity) { // copy constructor
        memcpy(arr, other.arr, size + 1);
        // memset and memcpy (по 8байтиков за раз умеют)
        // memmove - дает гарантию, что все байтики перенесутся в новую область, даже если области пересекались
    }
    // Компилятор умеет генерировать по умолчанию

    // Idiom Copy and Swap
    String& operator=(String other) {
        swap(other);
        return *this;
    }

    void swap(String& other) {
        std::swap(arr, other.arr);
        std::swap(size, other.size);
        std::swap(capacity, other.capacity);
    }

    ~String() {
        delete[] arr;
    }
};

int main() {
    {
        String s{15, 'a'};
        // s.~String();
        // Вот это UB
        // потому что будет double free одного и того же объекта
        // LifeTime заканчивается у объекта
    }
    {
        String s{15, 'a'};
        String s2 = s; // Вот это double free
    }
    {
        String s5 = s5;
        // Вот этот кринж - это действительно копирование строки от себя самой
        // UB
        // Непроиниц поля будут использоваться в копировании полей
    }
    {   //* Rule of three and assignment
        /*
            Оператор присваивания тоже генерируется компилятором по умолчанию
            Поэтому при написании своего оператора копирования, имеет смысл
            писать и свой оператор присваивания

        *   Если в классе есть нетрививальный конструктор
        *   | Copy | operator= | Destructor |
        *   , то они должны быть обязательно все написаны
        */
        String s(5, 'a');
        String s1 = {2, 'b'};
        // s = s1;
        // Будет double freee, поскольку тут тоже копируются просто поля класса
        // Если бы оператора присваивания не было
    }

}