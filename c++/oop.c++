//类和对象
/*
#include <iostream>

class Rectangle
{
    private:
    int width=0;
    int height=0;

    public:

    void setWidth(int a)
    {
        if(a<=0) return;
        width=(a<0)? 0:a;
    }

    void setHeight(int b)
    {
        if(b<=0) return;
        height=(b<0)? 0:b;
    }

    int area()
    {
        return height * width;
    }

   void show()
   {
    std::cout <<"长*宽=面积" <<area()<<std::endl ;
   }
};

int main()
{
    //栈上
    Rectangle a1;
    a1.setHeight(3);
    a1.setWidth(4);
    a1.show();
    a1.area();

    //堆上
    Rectangle *p=new Rectangle;
    p->setHeight(5);
    p->setWidth(6);
    p->show();
    p->area();

    delete p;
    return 0;
}
*/



//构造函数--对象一出生就自动执行，把成员收拾干净

/*
#include <iostream>
#include <string>

class Student
{
    std::string name;
    int score;

    public:
    Student()// 默认构造
    {
        name="无名";
        score=0;
    };
    Student(std::string n,int s) // 有参构造
    {
        name=n;
        score=s;
    };
    void show() { std::cout << name << " " << score << std::endl; }
};

int main()
{
    Student a; // 调默认构造
    Student b("MJC", 95);             // 调有参构造
    Student c = Student("TJX", 90);   // 同上的另一种写法
    Student* p = new Student("AB", 88);  // 堆上：new 自动调有参构造

    a.show();   // 无名 0
    b.show();   // MJC 95
    c.show();   // TJX 90
    p->show();  // AB 88   ← 注意堆对象用 -> 访问
    delete p;
    return 0;
}
*/



//函数名和类名完全相同；

//没有返回值（连 void 都不写，写了就错）；

//自动调用——创建对象时编译器自动触发，不能像普通函数那样手动调。
/*
#include <iostream>
#include <string>

class Student
{
public:
    std::string name;
    int score;

    Student()//默认构造
    {
        name="无名";
        score=0;
    }

};
int main(){
Student s;
std::cout <<s.score;//函数自动调用
}
*/



//初始化列表

/*
#include <iostream>

class Struct
{
    const int id;//const 成员：出生后不许再改，所以必须"出生时就装好"
    int &r;//引用成员 必须出生时绑定

    public:
    Struct(int x,int &ref) : id(x),r(ref) { };
    void show() { std::cout << "id=" << id << " r=" << r << std::endl; }
};

int main()
{
    int num=40;
    Struct a(15,num);
    a.show();
    num=100;
    a.show();
    return 0;
}
*/


//列表书写顺序永远和声明顺序保持一致
/*
#include <iostream>

class T
{
    int a;
    int b;

    public:
    T(int x) : a(x),b(a) { };

    void show()
    {
        std::cout <<a <<" " <<b<<std::endl;
    }
};

int main()
{
    T c(4);
    c.show();
    return 0;
}
*/




//析构函数

//写法：~类名()，无参数、无返回值、不能重载（一个类只有一个）；

//时机：栈对象出作用域时、堆对象 delete 时、全局对象程序结束时——自动调用；

//用途：归还资源。
/*
#include <iostream>
#include <string>

class Buf
{
public:
    int * data;
    
    Buf() {data=new int[100];std::cout <<"构造\n";}
    ~Buf()
    {
        delete []data;
        std::cout <<"释放内存\n";
    };
};

int main()
{
    {
        Buf b;
        std::cout << "--- b 活着，随便用 ---\n";
    }
    std::cout << "--- 出了大括号，内存已还 ---\n";
    return 0;
}


*/




//浅拷贝与深拷贝

//拷贝构造函数  照着一个已有的对象，造一个新对象
/*
#include <iostream>
#include <string>

class Score 
{
    std::string name;
    int score; 
public:
    Score(std::string n,int s) :name(n),score(s) 
    {std::cout << "普通构造: " << name << " " << score << std::endl;};
    Score (const Score &other) :// ← 拷贝构造函数
    name(other.name),score(other.score) 
    {
        std::cout << "拷贝构造: " << name << " " << score << std::endl;
    }

};

    void f(Score s) {std::cout << "f() 里用到了: " << std::endl;}
int main()
{
    Score a("mjc",100);
    Score b=a;// ① 用 a 初始化 b（最常见）
    Score c(a);// ② 同义的另一种写法
    f(a);// ③ 值传参：实参拷贝给形参时    此刻调用拷贝构造

    return 0;
}

*/




//深拷贝

/*
#include <iostream>

class Buf
{
public:
    int *data;
    Buf()
    :  data(new int[10]) 
    {
        for (int i=0;i<10;i++)
        {
            data[i]=i;
        }
    }

    // 深拷贝构造：三步走 —— new 自己的 → 抄内容 → （析构时各删各的）
    Buf(const Buf& other) :data(new int(10))
    {
    for (int i=0;i<10;i++)
        {
            data[i]=other.data[i];
        }
    }
    
    ~Buf() {delete []data;}

    void set(int idx,int val)
    {
        data[idx]=val;
    }

    int get(int idx) {return data[idx];}
};


int main()
{
    Buf a;
    Buf b=a;
    b.data[0]=99;
    std::cout << b.data[0] <<std::endl;
    return 0;
}

*/





//运算符重载

//operator=/operator+


/*
#include <iostream>

class Vec2
{
    int x,y;
public:
    Vec2 ( int a = 0 , int b = 0 ) : x (a), y (b) {}

    Vec2 operator+(const Vec2&other) const
    {
        return Vec2(x+other.x,y+other.y);
    }

    bool operator==(const Vec2&other) const
    {
        return x == other.x && y == other.y;

    }

    void show () { std::cout << "(" << x << ", " << y << ")" ; }
};

int main()
{
    Vec2 a(1,2),b(2,5);
    Vec2 c=a+b;
    c.show() ;
    Vec2 d(1,2);
    std::cout <<(d==a) <<std::endl;
    return 0;
}
*/

/*
#include <iostream>

class Counter
{
private:
    int *p;
public:
    Counter(int n) :p(new int(n)) {}

    Counter(const Counter&other) :p(new int(*other.p)) {}

    Counter&operator=(const Counter&other)
    {
        if(this==&other) return *this;
        delete p;
        p=new int(*other.p);
        return *this;
    }

    ~Counter() {delete p;}

    void set(int n)
    {
        *p=n;
    }

    int get()
    {
        return*p;
    }        
};

int main()
{
    Counter a(10),b(20),c(30);

    std::cout << "初始: a=" << a. get () << " b=" << b. get () << " c=" << c. get () << std::endl;

    b = a;
    std::cout << "b=a 后: a=" << a. get () << " b=" << b. get () << std::endl;

    a = a;
    std::cout << "a=a 后: a=" << a. get () <<std:: endl;

    c = b = a;
    std::cout << "c=b=a 后: a=" << a. get () << " b=" << b. get () << " c=" << c. get () <<std:: endl;

    b. set ( 99 );
    std::cout << "改b后: a=" << a. get () << " b=" << b. get () << " c=" << c. get () <<std:: endl;
    return 0;
}

*/







//静态成员 static

/*
#include <iostream>
class BankAccount
{
private:
    int balance;

public:
    static int count;
    BankAccount(int h) :balance(h){ count++;}
    ~BankAccount()  {count--;}

    static int getcount()//静态成员函数：没有 this 的函数
    {
        return count;
    } 
    void show()
    {
        std::cout <<this->balance <<std::endl;
    }
};
    int BankAccount::count=0;//类名::函数()

    
int main()
{
    BankAccount a1(1000);
    std::cout <<a1.getcount() <<std::endl;
    a1.show();
    return 0;
}
*/





//友元
/*
#include <iostream>

class BankAccount
{
private:
    double balance;
public:
    BankAccount(double b) :balance(b) {};
    ~BankAccount() {};
    friend void audit(const BankAccount & acc);
};

void audit(const BankAccount&acc)
{
    std::cout << acc.balance <<std::endl;
}

int main()
{
    BankAccount acc(5000);
    audit(acc);//函数(对象)
    return 0;
}

*/

//友元类和友元成员函数
/*
#include <iostream>
class wallet
{
    double balance=1000;
public:
    friend class audit;
};

class audit
{
public:
    void check(const wallet &w) {std::cout <<w.balance <<'\n';}
    void tax(wallet &w) { w.balance *=0.9;}
};   

int main()
{
    wallet w;
    audit a;
    a.check(w);
    a.tax(w);
    a.check(w);
    return 0;
}
*/












//杀手级应用：operator<<
/*
#include <iostream>
class Vec2
{
private:
    int x,y;
public:
    Vec2(int x,int y) :x(x),y(y) {};
    friend std::ostream& operator<<(std::ostream&os,const Vec2&v);
};

    std::ostream &operator<<(std::ostream&os,const Vec2&v)
    {
        os <<"(" << v.x << ", " << v.y << ")" <<'\n';
        return os;// 返回 os，才能连续 <<
    }


int main()
{
    Vec2 a1(3,4);
    std::cout <<a1 <<'\n';

    return 0;
}

*/





//const成员函数
/*
#include <iostream>

class Box {
    int w, h;
public:
    Box(int a, int b) : w(a), h(b) {}

    int area() const {        // 承诺：只读
        // w = 10;            // ✗ 编译报错
        return w * h;
    }
    void setW(int v) { w = v; }   // 普通函数：能改
};

int main() {
    const Box b1(2, 3);
    std::cout << b1.area() << "\n";   // 6 ✓
    // b1.setW(5);                    // ✗ 编译报错！

    Box b2(4, 5);
    b2.area();    // ✓ 普通对象来者不拒
    b2.setW(6);   // ✓
    return 0;
}
*/


//mutable const里面的特殊情况，给予修改的权力
/*
#include <iostream>

class Query {
    int data = 42;
    mutable int hits = 0;    // 后门：const 函数里也能改
public:
    int get() const {        // 逻辑上"只读"
        hits++;              // 内部偷偷记查询次数
        return data;
    }
    int get_hits() const{return hits;}
};

int main()
{
    Query q1;
    std::cout <<q1.get() <<'\n'<< q1.get_hits() <<'\n';
    return 0;
}

*/















