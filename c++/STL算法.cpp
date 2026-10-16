//STL算法

/*
#include <iostream>
#include <algorithm>
#include <vector>
//[begin(),end())左闭右开

//for_each(起点, 终点, 动作)
int main()
{
    //Lambda——动作就地写 ：
    std::vector<int>v={5,6,1,7,8,2};

    std::for_each(v.begin(),v.end(),[](int x) 
    {std::cout <<x <<" ";});
    
    //引用直接修改数据
    int sum=0;
    std::for_each(v.begin(),v.end(),[&sum](int x) { sum+=x;});
    std::cout <<"sum=" <<sum <<std::endl ;

    std::for_each(v.begin(),v.end(),[](int &x){x*=2;});
    for(auto &a:v) std::cout <<a <<" ";


    auto add = [](int a, int b) { return a + b; };
    //          |  │        │   │
    //          │  │       │   └─ 函数体：跟普通函数一模一样
    //          │   │        └─ 参数列表：跟普通函数一模一样
    //          │   └─ 捕获列表：从外部"带东西进来"传=（值）/&（引用）是把数据全部传进来
    //          └─ lambda 是个"值"，用 auto 存进变量

// 实测: 
    std::cout <<"add=" <<add(3,4) <<" ";


    return 0;
}
*/




//lambda表达式
/*
#include <iostream>
#include <algorithm>
#include <string>
#include <vector>

typedef struct student
{
    std::string name;
    int id;
    int scores;
}STU;

int main()
{
    std::vector<STU> stu=
    {
        {"tjx",1001,80},
        {"mjc",1002,100},
        {"lzh",1003,60}
    };

    //lambda表达式
    std::sort(stu.begin(),stu.end(),
    [](const STU&a,const STU&b){return a.scores > b.scores;});

    for(auto&s:stu)
    std::cout <<s.name <<" "<<s.id <<" "<<s.scores <<std::endl ;
    return 0;
}

*/





//find_if 找到第一个符合要求的
//count_if 找到符合要求的个数
// remove_if/erase 删除某个值
/*
#include <iostream>
#include <algorithm>
#include <string>
#include <vector>

typedef struct Student
{
    std::string name;
    int score;    
}Student;

int main()
{

    std::vector<int>v={5,6,7,2,6,4};
    //find 按值查询
    auto it=std::find(v.begin(),v.end(),7);
    if(it!=v.end()) std::cout << "下标="<<(it-v.begin()) <<std::endl;

    //find_if 找到第一个符合的
    std::vector<Student> stus = {{"张三", 55}, {"李四", 88}, {"王五", 92}};

    auto passIt = std::find_if(stus.begin(), stus.end(),
        [](const Student& s) { return s.score >= 60; });   
    std::cout <<passIt->name <<std::endl ;

    //count count_if
    std::cout << std::count(v.begin(), v.end(), 7)<<std::endl;                        // 实测: 1（有几个 7）
    std::cout << std::count_if(v.begin(), v.end(),
        [](int x) { return x % 2 == 0; })<<std::endl;

    //remove_if erase
    v.erase(std::remove_if(v.begin(),v.end(),[](int x){return x%2==0;}),v.end());
    for(auto &s:v) std::cout <<s <<" ";
    return 0;
}

*/



//sort 铁律： 比较规则只用`<` 或`>`
/*
#include <iostream>
#include <algorithm>
#include <vector>

typedef struct STU
{
    std::string name;
    int age;
    int scores;    
}STU;

int main()
{
    std::vector<int>v={3,5,4,1,8};
    std::sort(v.begin(),v.end());// 默认升序
    for(auto&s :v) std::cout <<s <<" ";

    std::sort(v.begin(),v.end(),[](int a,int b){return a>b;});
    for(auto&s :v) std::cout <<s <<" ";

    std::vector<STU>stu={{"张三",30, 55}, {"tjx", 21,88}, {"王五", 40,92},{"mjc",20,88}};
    
    std::sort(stu.begin(),stu.end(),//同分相遇
    [](const STU&a,const STU&b){if(a.scores!=b.scores )return a.scores > b.scores;
    return a.age >b.age;});

    for(auto&a :stu) std::cout <<a.name <<" "<<a.age <<" "<<a.scores <<std::endl ;

    return 0;
}

*/





































