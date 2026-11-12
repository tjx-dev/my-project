#include <iostream>
#include <mutex>// std::mutex、std::lock_guard
#include <thread>//std::thread

const double TIME=1000000;
int count=0;

std::mutex
g_mtx;//一把全局锁

void work()
{
    for(int i=0;i<TIME;i++)
    {
        std::lock_guard<std::mutex>
        g(g_mtx);
        count++;
    }

}

int main()
{
    std::thread t1(work);//招人干活
    std::thread t2(work);
    t1.join();//等干活完
    t2.join();
    std::cout <<count ;
    return 0;

}

















