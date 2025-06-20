#include <iostream>

/*
* Types of exceptions
    * Segmentation fault
        - array out of bounds
        - nullptr dereference
        - stack overflow
    * FPE
        - / 0
    * Abort
        - std::abort()
            - false assertion
            - std::terminate
        - std::terminate
            - pure virtual function call
            - uncaught exception

* Exception handling
    Исключения - очень дорого (выделение дин памяти + накладные расходы)

    Если new кинул исключение, то в статической памяти уже выделяется место
    под bad_alloc

    Если исключение не ловится, то это UB
    Значит деструктор не обязательно вызовется

* Multiple catch
    В catch не случается приведения типов
    Только полное соотвествие

    * Но только const
    * И по ссылке на родителя

* RAII
    При исключенияъ у объекта не вызывается деструктор
    А значит при выделении памяти на что-то и обработке исклчения
    будет утечка

    Короче RAII - это теорема как способ борьбы с утечкой памяти
    при исключениях путем использования smart pointers
    разных видов

    * Resource Acquisition is Initialization

* Exceptions in constructors and destructors
    * Exception in constructor
    function-try block
    можно еще при инициализации полей ловить exc с помощью try

    void f(...) try {
        ...
    } catch {
        ...
    }

    вот также можно и с конструктором
    в момент входа в catch
    все уже удалено: все созданные поля и классы родителей
    даже вызвался деструктор, если есть делегирующий и неделегирующий (в котором вылетел exc)

    * Exception in destructor
    До с++ 11 нельзя было выкидывать исключения из деструктора
    если деструктор был вызван по причине исключения
    С с++ 11 в целом нельзя выкидывать исключения из деструктора ВСЕГДА

    есть волшебное noexcept(false) с помощью которого можно выкидывать исклячение
    но нельзя при первом условие, когда деструктор был вызван по причине исключения

    есть такое std::uncaught_exception()
    True - если сейчас летит eexception
    False - если нет
    Начиная с с++ 17 exceptionS
    бывает, что исключения одновременно летит 2 и более
*/

struct A {
    A() { std::cout << "A"; }
    A(const A&) { std::cout << "copy"; }
    ~A() { std::cout << "~A"; }
};

void f(int x) {
    A a;
    std::cout << &a << " \n";
    if (x == 0) {
        throw a;
        // когда бросаем - всегда копируем
        // кладем в дин память, а из стека удаляем старый объект
    }
}

// --------

struct Mom {};
struct Son : Mom {};

// ---------------------------------
//* RAII
// сделаем обертку!
struct Wrapper {
    int* p;
    Wrapper(int* p): p(p) {}
    Wrapper(const Wrapper&) = delete;
    Wrapper& operator= (const Wrapper&) = delete;
    ~Wrapper() { delete p; }
    int& operator*() {
        return *p;
    }
};
// Wrapper - он же smart pointer (unique ptr)
// Ну и сделаем не на int, а на любой, как шаблон
//* template <typename T>
//* это unique ptr
// --------

void g(int y) {
    if (y == 0) {
        throw 1;
    }
}

void g(Wrapper p) {
    if (*p == 0) {
        throw 1;
    }
}

void f1(int x) {
    // int* p = new int(x);
    Wrapper w(new int(x));
    g(*w);
    // delete p;
}

// ---------------------------------
//* Exceptions in constructors

struct A2 {
    A2() { std::cout << "A2"; }
    ~A2() { std::cout << "~A2"; }
};

struct S {
    A2 a;
    S(int x) {
        std::cout << "S";
        if (x == 0) throw 1;
    }
    /*
     можно еще вот так ловить сразу при инициализации
     S(int x) try: a(1), aa(1), aaa(0) {
        std::cout << "S";
     } catch (...) {
        ...
     }
    */
    ~S() {
        std::cout << "~S";
        // delete a;
        // вот это утечка памяти
        // ведь деструктор не вызваля
    }
};

int main() {
    {   try {
            S s(0);
        } catch (...) {

        }
        // вызвался конструктор A S и деструктор S
    }
    {
        f(5);
    }
    {
        try {
            f(0);
        } catch (...) {
            std::cout << "caught!";
        }
        // уничтожится А из дин памяти
    }
    {
        try {
            f(0);
        } catch (A a) {
            std::cout << "caught! " << &a;
        }
        // копирование еще и в момент когда ловим + когда пробрасываем
    }
    {
        try {
            f(0);
        } catch (A& a) {
            std::cout << "caught! " << &a;
        }
        // адрес уже объекта не на стеке, а в дин памяти
    }
    {
        try {
            try {
                // new int[400'000'000'000'000];
            } catch (A& a) {
                std::cout << "caught! " << &a << '\n';
                throw;
            }
        } catch (A& a) {
            std::cout << "caught again!" << &a << '\n';
        }
    }
    {
        std::cout << std::endl;
        try {
            throw 1;
        } catch(double d) {
            std::cout << "double";
        } catch (long long l) {
            std::cout << "long long";
        } catch (...) {
            std::cout << "other";
        }
    }
    {
        try {
            Son s;
            throw s;
        } catch (Son) {
            std::cout << "Son\n";
        } catch (Mom) {
            std::cout << "Mom\n";
        } catch (...) {
            std::cout << "other\n";
        }
        // при обычном наследовании Mom и Son (что раньше)
        // при private только Son
    }
}