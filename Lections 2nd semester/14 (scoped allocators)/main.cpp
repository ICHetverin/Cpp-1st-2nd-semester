#include <iostream>
#include <scoped_allocator>

/*
* Scoped allocators
    Ситуация ниже в примере
    У нас есть вектоор с кастомным аллокаторо и есть элементы
    - тоже векторы (строчки), у которых тоже есть свой аллокатор кастомный
    тип аллокатора один, но на каждую сущность будет отдельный аллокатор
    что как бы ну не очень хорошо

    * Решение
    * scoped_allocator_adaptor

* Attributes
    [[...]]
    *- [[no_unique_address]]
    *- [[no_discard]]           (warning, если со значением функции ничего не происходит)
    (например, operator new, allocate, empty (по смыслу схожа с очищением контейнера))
    *- [[likely/unlikely]]
    *- [[assume ... (x > 0)]]
*/

template <typename Alloc>
struct scoped_allocator_adaptor {
    Alloc alloc;

    template <typename T, typename... Args>
    void construct(T* ptr, const Args&... args) {
        if constexpr (std::uses_allocator_v<T, Alloc>) {
            using InnerAlloc = typename T::allocator_type;
            alloc.construct(ptr, args..., InnerAlloc(alloc));
        } else {
            alloc.construcy(ptr, args...);
        }
    }
};

template <typename T>
using MyAlloc = std::allocator<T>;

int main() {
    {
        using MyString = std::basic_string<char, std::char_traits<char>, MyAlloc<char>>;
        MyAlloc<MyString> alloc;
        // std::vector<MyString, MyAlloc<MyString>> v(alloc);                                   вот так некруто
        std::vector<MyString, std::scoped_allocator_adaptor<MyAlloc<MyString>>> v(alloc); //    вот так круто
        // теперь вектор будет создавать все строки на одном аллокаторе
        v.push_back("abc");
        v.push_back("cde");
    }
}