#include <iostream>

/*
* Exception safety
    4 уровня:
        - 1: Никогда не бывает exception
        - 2: Когда бросает exception, то возвращается к моменту как было
        гарантия, что будет также, как до вызова функции
        - 3: Базова гарантия: нет УБ и инвариантность сохранилась
        но не факт что также как до этого
        - 4: Нет гарантии: УБ

    * noexcept
    Обязан поймать все на этом уровне!
    std::terminate если исключение пробрасывается дальше
    можно внутри нее проверить CT условие

    оператор noexcept (проверяет в СТ)

    noexcept помечаются ф-ции в которых вообще ничего плохого не может случиться
    Деструктор - noexcept по умолчанию
*/

void f() noexcept {}

template <typename T>
void f1() noexcept(std::is_reference_v<T>) {}

template <typename T>
void f() noexcept(noexcept(f1T>())) {};

int main() {
    {
        std::cout << noexcept(f());
    }
}