#include <iostream>

/*
    *Constants and cons refs and Lifetime expansion

    *Const
    модификатор типа, который делает знач неизм
    просто другой тип, у которого часть функций родительского отсутсвует
    (можно так воспринимать)
    (ну нет операции присваивания, инкрементации)
    (сам тип это lvalue)

    *Const ref
    ! Нельзя оставить без инициализации

    !Никогда не стоит принимать в ф-ции
    значение по const
    оно все равно компируется
    и копию запрещается менять
    смысла особого нет

    * А вот щас важно
    В фции хотим допустим брать объект, которого нет
    ну типа
    f("abcd");
    Так вот по стандарту
    * const std::string& такое разраешает
    * std::string&  - нет, а конст - да
    поэтому кажется, что это самый выигрышный вариант
    Ну вот в нашем варианте f("abcd")
    происходит неявная конвертация const char[5] -> const std::string
    * и const std::string* разрешает брать и lvalue, и rvalue

    * Когда использование просто по переменной лучше (void f(int))
    (int, double, char, ..) не тяжелые типы!
    - Когда ничего особо тяжелого нет
    и ведь компилятор просто неявно передает пойнтер
    и этот пойнтер
    1) Весит как мин 8байт
    2) Разыменовыание пойнтера дороже по времени чем обращение просто к значению переменной
*/

std::string substr_(const std::string&, size_t from, size_t to);

// void fs(std::string); //* Ok
void fs(std::string&); //* Okk too + exact type matching
void fs(const std::string&); //* Most usable + conversion to const
// void fs(const std::string); //! Never pls

void g(size_t& y) {
    ++y;
}

const int& g(int x) {
    return x++;
}

int main() {
    {   // Для обратной совмести с С например да (где нет const)
        // работает это
        char* word = "abcd";
        // хотя тип то тут const char
    }
    {
        const int a = 5;
        // a = 6; // ошибка
        int b = a;
    }
    {
        int x = 5;
        int* p = &x;

        const int* pc = p; // вот это указатель на конст инт
        // int является константым, а pc - нет
        int* const cp = p; // вот это конст указатель на инт
        // int не явялется константным, a pc - да
        // Примеры что делать можно с ними
        const int* const cpc = p; // вот это просто все конст
        // и соотвественно изменять его никак не получится

        int y = 10;
        pc = &y; // так можно
        // *pc = y; // вот так нельзя
        //! error: read-only variable is not assignable
        *cp = y; // вот так можно
        // cp = &y; // вот так нет
        //! error: cannot assign to variable 'cp' with const-qualified type 'int *const'
    }
    {
        int x = 5;
        int* p = &x;
        const int* pc = p; // int* -> const int*
        // неявное преобразование в конст
        // int* p2 = pc; // const int* -> int*
        // неявное преобразование в не конст и так нельзя
    }
    {
        int x = 5;
        const int* p = &x;
        x++;
        std::cout << *p << std::endl; // 6
        // ну то есть const int* запрещает тебе менять сам *p
        // но изменения которые в принципе происходят с x
        // он отменить не может
    }
    {   //* Ссылки
        int x = 5;
        const int& r = x; // конст ссылка на х
        // лишаемся части прав с конст
        //++r; // так нельзя да
        // const int& ~ int const& - константная ссылка
        //! int& const - вот это уже ошибка компиляции
        // int& r1 = r; // вот так ошибка компиляции
        // нельзя давать доп права
        //! error: binding reference of type 'int' to value of type 'const int' drops 'const' qualifier
    }
    {
        // const int* p = new const int[10]; // вот так ошибка компиляции
        // поскольку эти 10 ссылок не проиниц
        //! error: default initialization of an object of const type 'const int[10]'
        // const int* p = new const int[10]{0}; // а вот так уже можно
        // они уже проиницализированы
        // int* p = new const int[10]{0}; // так тоже нельзя
        // ждемс const int* от new
    }
    {   //* Lifetime expansion
        const int& x = 5; // поскольку cons ссылка может быть проиниц rvalue
        // неявно создастся int в дин памяти
        // ну и то же самое с любими объектами (string, vector)
        // и удаляет объект уже тогда, когда ссылка выйдет из области видимости

        const std::string& s = "aaaaaaaa";
        /*
            Короче, что происходит:
            в data лежит этот "aaaaaa", как const char*
            далее создается объект строки
            3 числа (size, capacity, pointer)

            Они хранятся на стэке (24 байта)
            + на стэке где-то хранится "aaaaaa"
            на который и указывает pointer от стринги
        */
    }
    {   //* Конкретный пример, почему нельзя
        //* разрешить простым ссылкам продлевать жизнь объектам
        int x = 0;
        // g(x); // CE - не тот же тип
        std::cout << x << std::endl; // 0, Хотя ожидается ++x == 1
        /*
            во если бы продление жизни не по конст ссылкам
            то вот такое было бы разрешено и даже скомпилировалось
            * void g(size_t& y) {
            *     ++y
            * }
            тип совпадает, и поэтому
            создался бы временный size_t
            и изменился бы он (локальная копия)
            а не сам объект, который передавали
            ! Вот такие пироги !
        */
    }
    {
        std::string s = "abc";
        // f(s);
        //! error: call to 'f' is ambiguous
        // 3 mathcing types
        // (std::string&) (const std::string&) (const std::string)
        const std::string& rs = s;
        // fs(rs); // а вот так уже только (const std::string&)
    }
    {
        // Битая ссылка
        g(5);
    }
    {
        int* p = new int(8);
        int** pp = &p;
        // const int** cpp = pp;
        //! Нельзя неявно добавлять глубже чем на один уровень указателей
        /*
            Нельзя потому, что
            Исследуем ситуацию ниже

            Короче, что происходит
            есть конст чар х = а
            есть не конст пойнтер на нулл
            есть ку - не конст пойнтер на не конст пойнтер на конст чар
            задаем ку как адрес на пойнтер на нулл (пс)
            далее поскольку ку - неконст на неконст на конст
            то разыменовываем его и получаем
            получаем указатель неконст на конст чар
            указатель сам неконст -> соотв меняем его на адрес х
            далее получаем что пс уже указывает на х
            и меняем значение пс на б
            все
            поменяли значение у конст чара
        */
    //    const char x = 'a';
    //    char* pc = nullptr;
    //    const char** q = &pc; // q now points to p; this is an error
    //    //* можно кстати вот так:
    //    // const char* const* q - то есть два раза навесили конст на два уровня
    //    *q = &x;              // p now points to x;
    //    *pc = 'b';             // Ouch: modifies a const char!
    //    std::cout << x << std::endl;
    }
}
