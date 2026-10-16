//继承

/*
#include <iostream>
#include <iostream>

class Person
{
protected:// protected：儿子能直接碰，外人不能
    std::string name;
    int age;

public:
    Person(std::string n,int a) :name(n),age(a) {};
    void intro () { std::cout << name << " " << age << "岁\n" ; }
};

class Student :public Person// 语法：class 子类 : 继承方式 父类
{
    int score;
public:
    Student(std::string n,int a, int s) :Person(n,a) ,score(s) {};
    //                     ↑ 【必背】初始化列表里调"父类构造"
    //                      不能直接写 name(n)——name 是父类的领地
    void study()
    {
        intro();
        std::cout <<name <<"在学习\n";
    }
};

int main()
{
    Student s("mjc",18,100);
    s.study();
    return 0;
}

*/



//同名遮蔽 hiding
/*
#include <iostream>

class Person {
public:
    void say()      { std::cout << "人\n"; }
    void say(int x) { std::cout << "人" << x << "\n"; }
};
class Student : public Person {
public:
    //using Person ::say;办法一，全部释放
    void say() { std::cout << "学生\n"; }   // 子类出现一个 say
};

int main() {
    Student s;
    s.say();              // 实测输出：学生
    // s.say(5);          // ✗ 编译报错！连父类的 say(int) 也一起被藏了
    s.Person::say(5);     // 办法二 单个释放：人5 —— 类名:: 强行访问父类版本
}

*/






//菱形继承与虚继承（virtual）

/*
   //菱形继承
//        Animal
//       ↗      ↖
//   Horse      Donkey
//       ↖      ↗
//          Mule

#include <iostream>

class Animal
{
public:
    int age=0;
};

class Horse:public Animal{};
class Donkey :public Animal{};

class Mule:public Horse,public Donkey{};// 多继承写法：逗号隔开


int main()
{
    Mule m;
// m.age = 3;        // ✗ 编译报错：马的 age？驴的 age？说不清 = 二义性
    m.Horse::age=3;
    m.Donkey::age=2;
    std::cout <<m.Horse::age <<'\n';
    std::cout <<m.Donkey::age <<'\n';
    std::cout <<"Mule的字节数" << sizeof(Mule) <<'\n';//包含俩份int=8
    return 0;
}

*/




//菱形继承具有二义性和数据冗余，要通过虚继承来解决.虚继承出现在中间层
//虚继承
/*
#include <iostream>

class Animal
{
    public:
    int age=0;
};

class Horse : virtual public Animal{};
class Donkey : virtual public Animal{};

class Mule : public Horse,public Donkey{};

int main()
{
    Mule m;
    m.age=5;
    std::cout <<m.age <<'\n';
    std::cout <<sizeof(Mule) ;//Horse、Donkey 各揣的一个 虚基类指针 vbptr(8字节)
    return 0;
}

*/



//构造顺序  口诀：谁最派生，谁负责虚基类
/*
#include <iostream>

class Animal 
{
protected:
    int age;
public:
    Animal(int a) :age(a){}
    void showAge(){std::cout <<age <<'\n';};
};

class Bird :virtual public Animal 
{
public:
    Bird(int a):Animal(a){}
};

class Swimmer:virtual public Animal
{
public:
    Swimmer(int a) :Animal(a){}
};


class Duck :public Bird,public Swimmer
{
    public:
    // ★ 全题最关键的一行： 
    // Animal 没有 Animal()（无参构造），所以 Duck 作为最派生类， 
    // 必须亲自调用 Animal(a)，否则直接编译报错！ 
    // // Bird(99)、Swimmer(88) 随便传——里面写的 Animal(99)、Animal(88) 
    // 全部作废，不影响 age 的值
    Duck(int a):Animal(a),Bird(55),Swimmer(99){}
    //Duck里要包着完整的 Bird 部件和 Swimmer 部件
};

int main()
{
    Duck d(5);
    d.showAge();
    return 0;
}

*/




//多态

//多态条件： 
//有 继承 关系
// 父类函数标了 virtual
//用 父类指针 / 引用 去调用

/*
#include <iostream>

class Animal
{
public:
    virtual void speak(){std::cout <<"动物叫\n";}// virtual = 多态开关
    virtual ~Animal(){}
};

class Dog:public Animal
{
public:
// override：声明"我重写了父类虚函数"
    void speak() override{std::cout <<"汪汪叫\n";}
};

class Cat:public Animal
{
    void speak() override{std::cout <<"喵喵叫\n";}
};

int main()
{
    Dog d;
    Cat c;

    Animal *p=&d;//1.指针方法
    p->speak();

    Animal &r=d;//2.引用方法
    r.speak();

    Animal *q=&c;
    q->speak();

    Animal *zoo[]={&d, &c};
    //Animal *&a:zoo改数值用这个
    //auto，自定义类型Animal *
    for(auto *a:zoo) a->speak();// 对扩展开放、对修改关闭,只拷贝，修改不会改变值
    return 0;
}

*/



//虚函数
//① p 找到对象 → ② 读对象里的 vptr → ③ 查 vtable 的 speak 格 → ④ 跳到格子里的真函数


//纯虚函数=0与抽象类
/*
#include <iostream>

class Shape// 抽象类
{
public:
    virtual double area() const=0;//必背 纯虚函数=0 表示"没有实现，子类必须写"
    virtual ~Shape() {}
    void describe() const {std::cout <<area() <<'\n';}// 基类接口里直接调虚函数
};

class Cicrle :public Shape
{
    double r;
public:
    Cicrle(double R) :r(R) {}
    double area() const override {return 3.1415*r*r;}
};

class Rect :public Shape
{
    double width,high;
public:
    Rect(double w,double h) :width(w),high(h) {}
    double area() const override {return width*high;}
};


int main()
{
    Shape *p=new Cicrle(1);
    p->describe();
    delete p;

    Shape *shape[]={new Cicrle(8),new Rect(2,3)};
    for(auto *s :shape) s->describe();
    for(auto *q:shape) delete q;
    return 0;
}

*/

/*

//虚析构函数

class Base1 {                       // 不写 virtual
public:
    ~Base1() { std::cout << "~Base1\n"; }
};
class Der1 : public Base1 {
public:
    Der1()  { std::cout << "Der1 生\n"; }
    ~Der1() { std::cout << "~Der1\n"; }
};

class Base2 {                       // 写 virtual
public:
    virtual ~Base2() { std::cout << "~Base2\n"; }
};
class Der2 : public Base2 {
public:
    Der2()  { std::cout << "Der2 生\n"; }
    ~Der2() { std::cout << "~Der2\n"; }
};

int main() {
    Base1* p1 = new Der1;
    delete p1;    // 实测输出：Der1 生 / ~Base1     ← ~Der1 被跳过！
    Base2* p2 = new Der2;
    delete p2;    // 实测输出：Der2 生 / ~Der2 / ~Base2   ✓ 先子后父
}
*/












