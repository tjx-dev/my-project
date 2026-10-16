
/*
写一个 Score 类：

私有成员：string name、int score；
用初始化列表写有参构造 Score(string n, int s)；
补一个默认构造（name 为“无名”、score 为 0）；
析构函数打印 xxx 离开了（xxx 换成 name）；
main 里：栈上创建一个有参对象、{} 里再创建一个默认对象、堆上 new 一个有参对象并 delete；
*/
/*
#include <iostream>
#include <string>

class Score
{
    std::string name;
    int score;
public:
    Score() : name("无名") ,score(0) { }
    Score(std::string n, int s) : name(n),score(s) { }
    ~Score()   {std::cout <<name<<"离开了\n";}
};

int main()
{
    //栈上
    Score a("mjc",100);
    {
        Score b;
    }
    Score *c=new Score("tjx",60);
    delete c;
    return 0;
}
*/





/*写完整的`IntArray` 类（必考题型的标准形态）：

- 私有成员：`int* arr` 、`int n` ；
- 构造`IntArray(int size)` ：`new int[size]` ，元素全填 0；
- 手写深拷贝构造 ；
- 析构函数：`delete[]` ；
- `main` 里：创建大小 3 的`a` ，`b = a` 拷贝，
把`b[1]` 改成 99（提供`set(int idx, int val)` ），打印`a[1]` 和`b[1]` 证明互不影响
，最后正常结束不崩溃。*/

/*
#include <iostream>

class IntArray
{
private:
    int *arr;
    int n;
public:
    IntArray(int size) :arr(new int[size]),n(size) 
    {for (int i=0;i<size;i++) arr[i]=0;}

    IntArray(const IntArray&other)
    :arr(new int[other.n]),n(other.n) { for (int i=0;i<n;i++) arr[i]=other.arr[i];}

    ~IntArray(){delete []arr;}

    void set(int idx, int val)
    {
        arr[idx]=val;
    }

    int get ( int idx) 
    { return arr[idx]; }
};

int main()
{
    IntArray a(3);
    IntArray b=a;
    b.set(1,99);
    std::cout << a. get ( 1 ) << " " << b. get ( 1 ) << std::endl;
    return 0;
}
*/







/*

#include <iostream>
using namespace std;

class Buffer {
    int *data;
    int size;
public:
    Buffer(int n) : size(n) {
        data = new int[size];
    }
    Buffer(const Buffer &other) : size(other.size) {
        data = new int[size];
        for (int i = 0; i < size; i++) data[i] = other.data[i];
    }
    ~Buffer() { delete[] data; }
    void set(int i, int v) { data[i] = v; }
    int get(int i) { return data[i]; }

    // ← 在这里补 operator=，要求：
    //    1. 含自赋值检查
    //    2. 先释放自己管着的旧内存，再深拷贝（注意 size 也要更新）
    //    3. 返回 Buffer&，支持链式赋值
    Buffer& operator=(const Buffer&other)
    {
        if(this==&other) return *this;

        delete [] data;
        size=other.size;
        data=new int [size];
        for(int i=0;i<size;i++)
        {
            data[i]=other.data[i];
        }
        return *this;
    }
};

int main() {
    Buffer a(3);
    a.set(0, 10); a.set(1, 20); a.set(2, 30);

    Buffer b(1);            // b 已存在，管着一块长度 1 的旧内存
    b = a;                  // 触发 operator=
    b.set(0, 99);           // 改 b 不许影响 a
    cout << a.get(0) << " " << b.get(0) << endl;   // 期望: 10 99

    Buffer c(2);
    c = b = a;              // 链式赋值
    cout << b.get(0) << " " << c.get(2) << endl;   // 期望: 10 30

    c = c;                  // 自赋值：不许崩、内容不许变
    cout << c.get(1) << endl;                      // 期望: 20
    return 0;
}

*/


/*
#include <iostream>
#include <iostream>
class Student
{
    std::string name;
    int score;
public:
    Student(std::string n,int s ) :name(n),score(s) {};
    friend void compare (const Student&stu1,const Student&stu2);
};

    void compare(const Student&stu1,const Student&stu2)
    {
        int a=0;
        if(stu1.score==stu2.score) 
        {
            std::cout <<"一样高" <<'\n';
        }else 
        if (stu1.score > stu2.score) 
        { // 直接比，不用反查
            std::cout << stu1.name << " " << stu1.score<< " > " <<
            stu2.name << " " << stu2.score << "\n" ;
        } else
        {
            std::cout << stu2.name << " " << stu2.score
            << " > " << stu1.name << " " << stu1.score << "\n" ;
        }
    }
int main()
{
    Student s1("mjc",95),s2("tjx",60);

    compare(s1,s2);
    return 0;
}
*/




/*
#include <iostream>
#include <string>
class Cat {
    // ① private 成员：std::string name;  static int count;（猫总数）
private:
    std::string name;
    static int count;
public:
    // ② 带参构造 Cat(const std::string &n)：用初始化列表存 name，count++
    Cat(const std::string &n) :name(n) {count++;};
    // ③ 析构函数：count--
    ~Cat() {count--;};
    // ④ static 函数 howMany()：返回 count
    static int howMany() {return count;}
    // ⑤ const 成员函数 meow()：打印 name + "：喵"（不许改任何成员）
    void meow() const{std::cout <<name << ": 喵"<<'\n';}
    // ⑥ Cat& rename(const std::string &name)：
    //    参数名和成员变量同名——函数体里用 this 区分，把成员改成参数值
    //    并且返回 *this，让下面的链式调用能跑
    Cat&rename(const std::string&name)
    {
        this->name=name;
        return *this;
    }
};

int Cat::count=0;

int main() {
    {
        Cat c1("咪咪"), c2("团子");
        c1.meow();                              // 咪咪：喵
        std::cout << Cat::howMany() << "\n";         // 2
    }
    Cat c3("小白");
    c3.rename("大白").rename("老白");           // 链式改名
    c3.meow();                                  // 老白：喵
    std::cout << Cat::howMany();                     // 1
    return 0;
}
*/
/*
Animal：protected 成员 int age;，
有参构造 Animal(int a)，成员函数 showAge() 打印 age
Bird : virtual public Animal：构造 Bird(int a)，
初始化列表里也写了 Animal(a)
Swimmer : virtual public Animal：
构造 Swimmer(int a)，初始化列表里也写了 Animal(a)
Duck : public Bird, public Swimmer：
构造 Duck(int a)——初始化列表必须直接调 Animal(a)，
同时调 Bird 和 Swimmer 的构造（Bird、Swimmer 各传一个年龄参数，随便给）
main：Duck d(5); d.showAge(); 期望输出 5
*/
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
};

int main()
{
    Duck d(5);
    d.showAge();
    return 0;
}
*/



/*
#include <iostream>

class Base {
public:
    virtual ~Base() { std::cout << "~Base\n"; }   // 注意：构造没打印
};
class Mid : public Base {
public:
    Mid()  { std::cout << "Mid生\n"; }
    ~Mid() { std::cout << "~Mid\n"; }
};
class Leaf : public Mid {
    int* buf;
public:
    Leaf() : buf(new int[10]) { std::cout << "Leaf生\n"; }
    ~Leaf() { delete[] buf;    std::cout << "~Leaf\n"; }
};

int main() {
    Base* p = new Leaf;
    delete p;
    return 0;
}


*/

/*
#include <iostream>
#include <vector>
#include <algorithm>
int main()
{
    std::vector<int>v={};
    v.push_back(50);
    v.push_back(85);
    v.push_back(40);
    v.push_back(60);
    v.push_back(95);

    //最经典用法：删掉所有等于 60 的元素
    for(auto it=v.begin() ; it!=v.end();) 
    {
        if(*it<=60)
        {
            it=v.erase(it);
        }else
        ++it;

    }
    
    //范围for
    for(int a:v) std::cout <<a <<'\n';

    return 0;
}

*/

/*任务 1：去重统计 输出搜索词的 去重个数 （出现了多少个不同的单词）。

任务 2：热度分级输出 把所有不同的单词按以下规则输出，每个单词一行，格式为`单词 频率` ：

- 按 出现频率从高到低 排列；
- 频率相同的单词，按 字典序从小到大 排列。
任务 3：热度查询 对每个查询单词，输出它的出现频率；如果该单词从未出现过，
输出`NOT FOUND` 。
注意：n 和 q 都可能达到 10⁵， 查询部分必须做到平均 O(1) ，整
体超时就说明容器选错了。*/

/*
#include <iostream>
#include <vector>
#include <string>
#include <set>
int main()
{
    int n;
    std::cout <<"输入一共有多少个单词" <<std::endl;
    std::cin >>n;

    std::vector<std::string> v(n);
    std::cout <<"输入你要查询的单词\n";
    for(int i=0;i<n;i++)
    {
        std::cin >>v[i];
    }

    int m;
    std::cout <<"输入要查询的次数\n";
    std::cin >>m;

    //去重
    std::set<std::string> s;
    for(int i=0;i<n;i++)
    {
        s.insert(v[i]);
    }

    std::cout <<"不同个数="<<s.size() <<std::endl;
    
    return 0;
}
*/


/*
#include <iostream>
#include <algorithm>
#include <vector>

int main()
{
    std::vector<int> v{12, 7, 9, 25, 3, 18, 7, 40, 2};
    
    int n=std::count_if(v.begin(),v.end(),[](int x){return x>8;});
    std::cout <<"n=" <<n<<std::endl;

    auto it=std::find_if(v.begin(),v.end(),[](int x){return x%5==0;});
    if(it!=v.end()) std::cout <<"first=" <<*it <<std::endl;
    else std::cout << "没有5的倍数" << std::endl;

    v.erase(std::remove_if(v.begin(),v.end(),[](int x){return (x&1)==0;}),v.end());

    std::sort(v.begin(),v.end(),std::greater<int>());
    for (int x : v) std::cout << x << " ";
    return 0;
}
*/


/*
#include <iostream>
#include <algorithm>
#include <memory>
#include <vector>

struct Res {
    int id;
    explicit Res(int i) : id(i) { std::cout << "Res(" << id << ") 构造\n"; }
    ~Res() { std::cout << "Res(" << id << ") 析构\n"; }
    void hello() const { std::cout << "hello from " << id << "\n"; }
};

void take(std::unique_ptr<Res> p)
{
    p->hello();
}

int main()
{
    auto p=std::make_unique<Res>(1);

    take(std::move(p));
    if(nullptr==p) std::cout <<"p为空\n";

    std::vector<std::unique_ptr<Res>>R;
    R.push_back(std::make_unique<Res>(2));
    R.push_back(std::make_unique<Res>(3));

    return 0;
}


*/


#include <iostream>
#include <fstream>
#include <string>
#include <vector>

int main()
{
    std::vector<std::pair<std::string ,int >> 
    stu={{"Alice", 90}, {"Bob", 85}, {"Carol", 77}};

    std::ofstream fout("score.txt");
    if (!fout) 
    {                      // 写方向也要检查
        std::cout << "打开失败\n";
        return 1;
    }
    for(auto &[name,score] :stu)
    fout<<name  <<" "<<score<<std::endl;

    fout.close();

    std::string name;
    int score,n=0,sum=0;
    double avg=0;
    std::ifstream fin("score.txt");

    if(!fin)
    {
        std::cout <<"打开失败\n";
        return 1;
    }

    while(fin >>name >>score )
    {
        sum+=score;
        n++;
    }
    avg=(double)sum/n;
    std::cout <<"avg=" <<avg <<std::endl;
    return 0;

}

































































