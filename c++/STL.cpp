//顺序容器
//vector

/*
#include <iostream>
#include <vector>

int main()
{
    std::vector<int>v;
    int last=0;
    for(int i=0;i<16;i++)
    {
        v.push_back(i);

        if((int)v.capacity()!=last) // 只有搬家时 capacity 才变
        {
        std::cout <<"size=" <<v.size() <<'\n';
            std::cout <<"capacity=" <<v.capacity() <<'\n';
            last=(int)v.capacity();
        }
    }
    return 0;
}

*/

/*
#include <iostream>
#include <vector>
# include <algorithm> // sort 在这
int main()
{
    std::vector<int>v;
    v.reserve(4);//reserve是空的三个地方，resize是创建后全为0
    v.push_back(90);
    v.push_back(80);
    v.push_back(70);
    v.push_back(80);

    std::vector<int>v1={1,2,3,4,5};

    v.at(2)=75;//更加安全

    std::cout <<"scores.at(0)=" <<v.at(0) <<'\n';

   // scores.assign(3,2);//重灌，实测`assign(3,1)` → 3 个 1）
    std ::sort(v.begin(),v.end());// ① 默认升序

    v.erase(std::unique(v.begin(),v.end()),v.end());
    //vector：sort + unique 去重三步曲

    double sum=0;
    for(int s : v)//范围for
    {
        std::cout <<s <<'\n';
        sum+=s;
    }
    
    std::cout <<"平均分" <<sum/v.size() <<'\n';

    v.pop_back();//删除元素

    std::cout <<"size=" <<v.size() <<'\n' <<
    "last=" <<v.front() <<v[v.size()-1]<<'\n';
    //front/back首尾元素

    v.clear();//清空元素 size=0,capacity不变

    std::cout <<"size=" <<v.size() <<'\n';
    return 0;
}


*/



//erase的使用
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



//list 双向链表

/*
#include <iostream>
#include <list>

    int main()
    {
    // ── list：sort + unique 成员版 ── 
    std::list< int > c = { 5 , 1 , 5 };
    std::list< int > b = { 8 , 9 };
    c. sort (); // {1, 5, 5} //排序,不能使用std::sort
    c. unique (); // {1, 5}//暴力去重
    c. splice (c. end (), b); 
    // 把 {8,9} 剪接到尾巴 → {1,5,8,9} // b 空了
for ( int &x : c) std::cout << x << " " ; // 1 5 8 9 std::cout << "\n" ;
    std::cout << "b 的大小 = " << b. size () <<'\n'; // 0


    std::list<int>n={1,2,3,4,5,6};

    n.push_back(7);//尾插
    n.push_front(0);//头插

    auto it=n.begin();//=list<int>::iterator it=n.begin()
    it++;
    n.insert(it,99);
    n.sort(std::greater<int>());//降序
    for ( int &x : n) std::cout << x << " " ;

    return 0;
}

*/

//deque 双端队列
/*
#include <iostream>
#include <deque>

int main()
{
    std::deque<int> que={1,2,3,4,5,6};

    //list
    que.push_back(7);
    que.push_front(0);
    for(int &s:que) std::cout <<s <<" ";

    //vector
    std::cout <<que[2] <<'\n';

    return 0;
}
*/




//map
/*

#include <iostream>
#include <map>
#include <string>
#include <algorithm>

//有序，比较字符串首字母的ASCII大小
int main()
{
    std::map<std::string,int> m;

    //插入
    m.insert({"mjc",100});
    m["tjxiang"]=90;
    m.emplace("zbcd",80);
    
    m.at("mjc") = 95;      // key 存在 → 改 ✓
    //m.at("nothere") = 1;   key 不存在 → 抛 out_of_range 异常（大声报错）

    for(auto &[name,score]:m)
    {
        std::cout <<name << " "<<score <<std::endl;
    }

    //删除
    m.erase("zbcd");
    for(auto &[name,score]:m)
    {
        std::cout <<name << " "<<score <<std::endl;
    }

    //查找
    if(m.find("mjc")!=m.end())
    {
        std::cout <<"mjc=" <<m["mjc"] << std::endl;
    }else
    {
        std ::cout <<"no found"<<std::endl;
    }

    //count只有1/0
    if(m.count("mjc")) std::cout <<"true";
    return 0;
}
*/




//set
/*
#include <iostream>
#include <set>

int main()
{
    std::set<int>score;
    score.insert(10);
    score.insert(20);
    score.insert(5);
    score.insert(10);//去重

    for(auto &s:score)
    {
        std::cout <<s <<std::endl;
    }

    auto it=score.find(70);
    if(it!=score.end())
    {
        std::cout  <<*it<< std::endl;
    }else
    {
        std ::cout <<"no found"<<std::endl;
    }

    //删除
    score.erase(20);
    for(auto &s:score)
    {
        std::cout <<s <<std::endl;
    }
    return 0;
}

*/


//unordered_map,unordered_set
/*
#include <iostream>
#include <unordered_map>              // ← 改动①：<map> 换成 <unordered_map>
#include <string>

int main()
{
    // 无序：哈希表，key 按哈希值乱序存放，遍历顺序不可预测
    std::unordered_map<std::string, int> m;   // ← 改动②：类型名换掉

    // 插入（语法一模一样）
    m.insert({"mjc", 100});
    m["tjxiang"] = 90;
    m.emplace("zbcd", 80);

    // 修改（一样）
    m.at("mjc") = 95;              // key 存在 → 改
    // m.at("nothere") = 1;        // key 不存在 → 抛 out_of_range

    // 遍历（一样，但注意：输出顺序乱了！）
    for (auto& [name, score] : m)
        std::cout << name << " " << score << std::endl;

    // 删除（一样）
    m.erase("zbcd");
    for (auto& [name, score] : m)
        std::cout << name << " " << score << std::endl;

    // 查找（一样）
    if (m.find("mjc") != m.end())
        std::cout << "mjc=" << m["mjc"] << std::endl;
    else
        std::cout << "no found" << std::endl;

    if (m.count("mjc")) std::cout << "true";
    return 0;
}
*/




//stack
















































































