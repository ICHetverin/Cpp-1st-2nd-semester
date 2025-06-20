#include <iostream>
#include <vector>

/*
* Intro
    vector<int> v(10);
    int& x = v[5];
    int* p = &v[5];
    v.push_back(1);
    cout << *p;
    cout << x;

    Это УБ и называется pointer invalidation
    (при push_back pnt указывает на память, которая нам уже не принадлежит)
    х - тоже УБ (ссылка на недейств переменную)

    А вот в деке нет инвалидации указателей!

* Deque
    Есть массив указателей T** arr
    на массивы с сырой памятью

    Когда создается пустая дека - ничего не аллоцируется!
    Каждый массив - бакет (зарезервированная память)
    нужно знать пару
    i1 j1 - начало
    i2 j2 - конец

    Реаллокация происходит за счет увеличения массива указателей
    (но значения указателей там остаются прежними)

* stl Adapters
    - std::stack
        template<typename T, typename std::dequeue<T>>
    - std::queue
    - std::priority_queue

* Iterators and categories
    тип, который позволяет делать обход последовательности
    * 1) Разыменовывание
    * 2) Инкрементация
    * 3) Сравнение на равенство

    *- Input iterator
    *- Forward iterator (ходить по одной и той же посл-ти будет то же самое)
        (forward list, unordered set, unordered map)
    *- Bidirectional iterator (декрементация)
        (list, set, map)
    *- Random-access iterator (+= i; -= i; i1 - i2; i1 <= i2)
    *- Contiguous iterator (~ pointer)
        (since c++ 17)
        (vector, array, t*)
        *(&*it + n) == *(it + n)

* Range-based for
    когда проходимся по всем элементам посл-ти
    for (int x : a) { ... }

    по сути это то же что и: (при парсинге меняется на)
    for (std::vector<int>::iterator it = a.begin; it != a.end; ++it) {
        *it ...
    }

* Iterator traits
*/

template <typename InputIterator>
void find_mon(InputIterator begin, InputIterator end) {
    // auto x = *begin;
    // например для вектора булов - не будет работать
    // тогда
    typename std::iterator_traits<InputIterator>::value_type x = *begin;
    // вот это уже правильнее
}


int main() {
    {
        std::vector<bool> vb(10);
    }
    {
        std::vector<int> v = {1, 2, 3, 4, 5};
        for (int& x : v) {
            v.push_back(x);
        }
        for (int& x : v) {
            std::cout << x;
        }
        //! UB
        // ну при реалокации мы получаем что end никогда не будет достигнут
        // при range-based for end запоминается изначальный
    }
}