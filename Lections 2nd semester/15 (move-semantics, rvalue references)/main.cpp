#include <iostream>

/*
* Move semantics and rvalue references

* Rule of five
    рассмотрим push_back

    vector<string> v;
    v.push_back("abc");
    v.push_back(string("abc"))

    void push_back(const T&) {
        ...
        new(arr + n) T(value);
        / второй раз создание стринги из прошлой
    }

    template<typename... Args>
    void emplace_back(const Args&... args) {
        ...
        new(ptr) T(args...);
    }

    struct S {
        string data;
        S(const string& str) : data(str) {}
    };

    S("abc");

    ! Проблема в том, что нет способа проинициализировать поле data
    ! сразу "abc" без создания промежуточной строки

    ! В исключениях тоже копируется объект в дин память при throw

    * Для классов можно определить не Copy constructor а Move constructor
    получается, что объекты создаются просто забирая ptr на уже готовый, а не копировать

* Move constructor
    class string {
        char* arr;
        size_t sz_;
        size_t cap_;
        ...
        string(string&& other) :
            arr_(other.arr_), sz_(other.sz_), cap_(other.cap_)
        {
            other.arr_ = nullptr;
            other.sz_ = other.cap_ = 0;
        }
        string& operator=(string&& other) {
            delet[] arr;
            arr_ = other.arr_; other.arr_ = nullptr;
            sz_ = other.sz_;
            cap_ = other.cap_;
            other.sz_ = other.cap_ = 0;
            return *this;
        }

        вот как правильно
        void push_back(T&& value) {
            ...
            new(ptr) T(std::move(value));
        }
    };

* Rule of 5
    если реализован нетривиальный один из 5 операторов, то все нужно написать тоже

* Бывает, что нельзя копировать, но нужно мувать
    unique_ptr например

* От rvalue по возможности вызывается move
* От lvalue по возможности вызывается copy
    Можно заставить вызвать мув операцию
    через std::move

    struct S {
        string data;
        S(const string& data) : data(std::move(data)) {}
        / тут даже при std::move будет копирование

        S(string&& data) : data(std::move(data)) {}
        / тут уже будет move
    };
    S("abc");

    * тогда нужно 2 push back
    1)  void push_back(const T& value)
    2)  void push_back(T&& value) {
        ...
        new(ptr) T(std::move(value));
    }

* Exception
    since c++ 11
    string s = 'abc'
    throw s
    по возможности будет вызван move

* Swap
    Раньше:
    template <typename T>
    void swap(T& x, T& y) {
        T tmp = x;
        x = y;
        y = tmp;
    }

    Теперь: - работает за константу
    template <typename T>
    void swap(T& x, T& y) {
        T tmp = std::move(x);
        x = std::move(y);
        y = std::move(tmp);
    }
-----------------------------------------------
* Formal definitions of lvalue and rvalue
    Lvalue and rvalue are cateories of expressions not types!
    * Lvalue:
        - id
        - "abc"
        - =, +=, -=, *=, |=, ^=, ...
        - ++ex, --ex
        - *ptr
        - a[i]
        - comma if rhs is lv
        - ?: if both are lv
        - function call if return value is T&
        - cast-expression (same rule)
    * Rvalue:
        - literal (5, 'a', true)
        - +, -, *, /, &, |, ~, <, >, <=, >=, ^, &&, ||, !
        - ex++, ex--
        - &a
        - comma if rhs is rv
        - ?: if both are rv
        - function call if return value is T or T&&
        - cast-expression (same rule)

* T&& - rvalue reference and properties
    1) It is a rvalue function return value
    2) It can be initialized only with rvalue

* Reference qualifiers
    struct S {
        string str;
        string getData() const & { -> lvalue и rvalue
            return str;
        }

        string getData() && { -> доступен теперь только для rvalue
            return std::move(str);
        }

        string getData() & { -> доступен теперь только для lvalue
            return str;
        }
    }

    S{"abc"}.getData()

* Forwarding (universal) referencies and std::forward
    void push_back(const T&)
    void push_back(T&&)
    void emplace_back(const Args&... args)

    а теперь правильно
    void emplace_back(Args&&... args)

    void constract(U* ptr, Args&&... args) {
        new(ptr) U(std::forward<Args>(args...))
    }
*/

//* Реализация move ~ 90% случаев покрывает
template <typename T>
T&& move(T& x) {
    return static_cast<T&&>(x);
}
// неявного каста просто не бывает и смысл мува в касте к rvalue