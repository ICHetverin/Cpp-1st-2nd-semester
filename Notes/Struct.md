Структура это тап данных, объединяющий несколько более простых типов данных.
ПР 1:
```
struct name_of_struct{
	type fild1;
	type fild2;
	...
} struct_peremen1,struct_peremen2 ...;
```
ФИШКА:
определять экземпляры структуры можно сразу до её задания (struct_peremen1 ... в р=примере 1)

Обращение к полю ЭС (экземпляры структуры) реализуется через точку
```
name_of_struct struct_peremen1;
struct_peremen1.fild1;
```
```
struct l{
    int a;
    int s;
    char item[20];
};

int main(){
	l pr1;
	pr1.a=10;
	strcpy(pr1.item,"asdads");
}
```

Структуры с одинаковым типом данных можно копировать через равно 
```
int main(){

    l a1,a2;
    a1.a=a1.s=10;
    strcpy(a1.item,"asd");
    a2.a=a2.s=20;
    strcpy(a2.item,"zxcv");
    a2=a1;
    std::cout<<a1.a<<a1.s<<a1.item<<'\n';
    std::cout<<a2.a<<a2.s<<a2.item;
    return 0; 
///1010asd
///1010asd
}
```
Можно создавать массив структур
```
name_of_struct array_name[size];
```
можно и так 
```
struct name_of_struct{
	type fild1;
	type fild2;
	...
} struct_peremen1,array_name[size] ...;
```

Передача структуры в функцию происходит так же как и с обычными типами данных.

**Указатели на структуры**
создаются так же как и обычные, но чтоб обратиться к полю указателя используется стрелочка влево
```
l pr1;
l *ptr;
ptr=&pr1;
ptr->a=10;
```

так же может использоваться в качестве параметра для функции.
Так работает даже быстрее чем при полном копировании ф-и.

Пример с <ctime>
[[Пример использования укap на структуру с ctime]]
