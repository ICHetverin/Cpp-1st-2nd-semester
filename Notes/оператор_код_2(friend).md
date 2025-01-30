```
//gcc 7.4.0
#include <iostream>
#include  <stdio.h>

class th{
    int x,y,z;
    public:
    th():x(0),y(0),z(0){}
    th(int _x, int _y, int _z):x(_x),y(_y),z(_z){}
    friend th operator+(th ob1, th ob2);
    th operator=(th ob){
        x=ob.x;
        y=ob.y;
        z=ob.z;
        return *this;
    }
    th operator++(){
	x++;
	y++;
	z++;
	return *this;
    }
    th operator++(int i){
        th temp=*this;
        x++;
        y++,
        z++;
        return temp;
    }
    void show(){
        std::cout<<x<<'\n';
        std::cout<<y<<'\n';
        std::cout<<z<<'\n';
        std::cout<<'\n';
    }
    
};
th operator+(th ob1, th ob2){
    th temp;
    temp.x=ob1.x+ob2.x;
    temp.y=ob1.y+ob2.y;
    temp.z=ob1.z+ob2.z;
    return temp;
}
int main()
{
    th a(1,2,3),b(10,10,10),c;
    
    
    
    
    c = a + b; // сложение объектов 
    c.show();
    
    c = a + b + c; // множественное сложение объектов
    c.show();
    c=a=b; // множественное присваивание
    c.show();
    return 0;
}
```