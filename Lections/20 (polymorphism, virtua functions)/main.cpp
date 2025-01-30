#include <iostream>

/*
    * Polymorphism and Virtual functions

    Да, виртуальное наледование и виртуальные функции - конечно разные вещи

    Тип, у которого есть хоть одна виртуальная функция, называется полиморфным

    Чем отличается new от malloc - new вызывает еще и конструктор, помимо того,
    что просит выделить память на объект соотв типа

    ! Есть еще final
    Это ключевое слово запрещает всем дочерним переопределять такую функцию с точно
    такой же сигнатурой

    ! final уже значит под собой override
    ! override уже значит под собой virtual

    * Override and final - контекстно ключевые слова (можно определить переменную с таким именем)

    * Abstract classes and pure virtual functions
        Такие классы вооьще называются интерфейсами
        Это такие классы, у которых есть хотя бы одна функция, (pure virtual)
        которая не определена для этого класса
        Метод называется pure virtual, если он определен как virtual
        и равен 0
        ! Нельзя будет создать объект асбтрактного типа

    * Полиморфизм
    * Статический полиморфизм - это перегрузка функций
    * Динамический полиморфизм - это классы, которые наследуются, но их все можно положить в контейнер
    * под одним типом например (какая именно операция определяется in runtime)
*/

struct Base {
    // void f() {
    //     std::cout << 1 << std::endl;
    // }
    // virtual void f() {
    //     std::cout << 1 << std::endl;
    // }
    virtual void f() const {
        std::cout << 1 << std::endl;
    }

    virtual ~Base() = default;
    // Нужно, чтобы не было утечки памяти, если деструктор дочернего
    // не тривиавльный
};

struct Derived : Base {
    int* p = new int(0);
    // функция тоже становится автоматически виртуальной
    // поскольку имеет ту же сигнатуру, что и прошлая ф
    // void f() {
    //     std::cout << 2 << std::endl;
    // }
    // void f() override { // вот ошиблись в сигнатуре функции и сразу получили CE именно из-за override
    void f() const override { // вот так уже ОК
        std::cout << 2 << std::endl;
    }
    // Важно писать overried по код стайлу раз
    // Во-вторых именно это слово говорит о том, что мы явно
    // хотим явно переопределить функцию родителя
    //! Слово override нужно для того, и только для того, чтобы иногда
    //! ловит CE при компиляции, в случае если мы ошиблись в сигнатуре
    // Иначе можно ошибиться в сигнатуре и будут две функции, но дочерняя
    // не переопределит родительскую
    ~Derived() {
        delete p;
    }
};

struct Granny {
    virtual void f() const {
        std::cout << 1 << std::endl;
    }
};

struct Mom : Granny {
private:
    void f() const override {
        std::cout << 2 << std::endl;
    }
};

struct Son : Mom {
    void f() const final {
        std::cout << 3 << std::endl;
    }
};

struct Mom1 {
    virtual void f() {
        std::cout << 1 << std::endl;
    }
};

struct Dad1 {
    virtual void f() {
        std::cout << 2 << std::endl;
    }
};

// Просто структуру нельзя будет наследовать
struct Son1 final : Mom1, Dad1 {
    void f() override {
        std::cout << 3 << std::endl;
    }
};

// * Abstract Class
struct Shape {
    virtual double area() const = 0;
    // pure virtual function
};

struct Square : Shape {
    int a;
    Square(int a) : a(a) {}
    double area() const override {
        return a * a;
    }
};

struct Circle : Shape {
    int r;
    Circle(int r) : r(r) {}
    double area() const override {
        return M_PI * r * r;
    }
};

int main() {
    {
        Derived d;
        Base& b = d;
        b.f(); // 1
        // Вызвалась функция по адресу Base
    }
    {   // Но, если я хочу чтобы всегда при иниц объекта Derivedом
        // Вызывались функции именно от него, не смотря на то, какой тип у ссылки
        // Именно для этого и нужны виртаульные методы
        Derived d;
        Base& b = d;
        b.f(); // Вот при виртуальной функции будет 2
    }
    {
        Derived d;
        Base b = d;
        b.f(); // 1 потому что произошло просто копирование объекта
    }
    {
        Base* b = new Derived();
        delete b;
        // Вот это например утечка памяти, потому что delete
        // вызывает деструктор того типа, которым объявлена переменная, а не того
        // к кому кастится
    }
    {
        // Когда указали виртуальный деструктор для Base, то уже нет утечки памяти
        Base* b = new Derived();
        delete b;
    }
    {   //* Выводится 2!!!
        Mom d;
        Granny& b = d;
        b.f();
        /*
            Почему вывести 2 - логично
            Виртуальность - рантайм явление
            А приватность - компайлтайм

            Компилятор просто не знает какая функция будет выбрана - именно потому что
            Виртаульность - рантайм
        */
    }
    {
        // Mom m;
        // Granny g;
        // int x;
        // std::cin >> x;
        // Granny& gg = (x % 2 ? m : g); // Есть общий тип Granny
        // gg.f();
        // В компайлтайме нельзя выбрать функцию потому что они виртаульные
    }
    {
        Son1 s;
        Mom1& m = s;
        Dad1& d = s;
        m.f();
        d.f();
    }
    {   //* Polymorphism
        std::vector<Shape*> figures;
        figures.push_back(new Square(4.0));
        figures.push_back(new Circle(4.0));

        for (Shape* figure : figures) {
            std::cout << figure->area() << " ";
        }
        std::cout << '\n';

        // Вот это и называется полиморфизм
        // Языковое средсво, когда вы можете обращаясь к объекту одного типа
        // Получать разное в зависимости от того, что там реально находится под этим типом
    }
}
