#include <iostream>

/*
    Лексический парсер ищем и делит все написанное на лексемы - слова, выражение и тд
    Есть приоритет - строится семантическое связывание от самого низкого уровня

    token - осмысленная единица для компилятора
    expression - имеют иерархическую структуру

    ++x++ - семантическая ошибка например

    CE - Compiler Error
    RE - RunTime Error (seg fault)
    Abort

    *Types of UB:
    1) ill-formed diagnosible (компилятор может обнаружить) семантика
    2) ill-formed non-diagnosible (компилятор может не обнаружить) семантика
    3) undefined behaviour (самое страшное - вообще не знаем)
    4) unspecified behaviour (есть рамки)
    5) implementation-defined behaviour (зависит от компилятора/локали)
*/

int main() {
    // \\; - лексическая ошибка
    {
        // std::vector<int> v(10);
        // v[500000] = 11; - segfault
    }
    {
        // Terminated - Abort
        // std::vector<int> v(10);
        // v.at(50) = 20;
    }
    {
        for (int i = 0; i < 300; ++i) {
            std::cout << i << " " << i * 12345678 << "\n"; // если отдать компилятору, то он
            // скажет, что i всегда < 174, чтобы не было переолнения и цикл -> бесконечный
        }
    }
}