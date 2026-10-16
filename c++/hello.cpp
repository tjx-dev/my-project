//endl表示换行
//全局变量，静态变量，常量是在全局区
//局部变量，函数参数，返回地址是在栈上，地址不能外传
//动态创建的变量是在堆上面，要自己手动管理

/*
#include <iostream>
using namespace std; 

double average(int arr[],int n)
{
    double sum=0;
    for(int i=0;i<n;i++)
    {
        sum+=arr[i];
    }
    return sum/n;
}


int main()
{
    const int num=10;

    int *arr=new int[num];

    for (int i=0;i<num;i++)
    {
    cin >>arr[i];
    }


    cout << average(arr,num)<< endl;
    
    delete[] arr;
    return 0;
}

*/

//读入 n，再读入 n 个整数，输出这组数的最大值和平均值（保留 1 位小数）

/*

#include <iostream>
#include <iomanip>   // 用于 fixed 和 setprecision


double max(int arr[],int n)
{
    int max=0;
    for(int i=1;i<n;i++)
    {
        if(arr[i]>arr[max])//假设0是最大的    
        {
            max=i;
        }
    }
    return arr[max];
}

double average(int arr[],int n)
{
    double sum=0;
    for(int i=0;i<n;i++)
    {
        sum+=arr[i];
    }
    return sum/n;
}


int main()
{
    int n;
    std::cout <<"输入你要输入的个数\n";
    std::cin >>n;
    int *p= new int[n];
    std::cout <<"输入数据\n";
    for(int i=0;i<n;i++)
    {
        std::cin >>p[i];
    }

    std::cout << max(p,n) <<std::endl;
    std::cout <<std::fixed<<std::setprecision(1)<< average(p,n)<<std::endl;
//fixed：固定小数位数
//setprecision(1)：保留 1 位小数

    delete[] p;
    return 0;
}

*/


//读整行要用 getline(cin, s),要和#include <string>一起
//cin.ignore();           // 吃掉残留的 '\n'

/*
#include <iostream>
#include <string>

int main()
{
    std::string p;
    int n;
    std::cout <<"输入你要输入的数字\n" <<std::endl;
    std::cin >>n;

    std::cin.ignore();//吃掉空格
    std::cout <<"输入城市名字\n";
    int total=0;
    for(int i=0;i<n;i++)
    {
        std::getline(std::cin,p);
        if(i==n-1)
        {
            std::cout << p <<std::endl;
        }
        total+=p.size();
    }
    std::cout <<total<<std::endl;
    return 0;
}

*/




/*
#include <iostream>

void swapInt(int& a, int& b)
{
    int t=a;
    a=b;
    b=t;
}

void printInfo(const std::string& name, int times = 2)
{
    for(int i=0;i<times;i++)
    {
        std::cout <<name ;
    }
    std::cout <<std::endl;
}


int main()
{
    int a,b;
    std::cin >>a;
    std::cin >>b;
    swapInt(a,b);

    std::cout <<"交换后的数字\n";
    std::cout << a<<" " << b<<std::endl;
    printInfo("MJC");
    printInfo("TJX",5);
    return 0;

}
*/














