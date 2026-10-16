//智能指针

//unique_ptr
/*
#include <iostream>
#include <memory>


class Cat
{
public:
    Cat() {std::cout <<"start" <<std::endl;}
    ~Cat(){std::cout <<"end" <<std::endl;}

    void sound(){std::cout <<"喵喵" <<std::endl;}

};

void check(const std::unique_ptr<Cat>&s)
{
    if(s!=nullptr) std::cout <<"had lived"<<std::endl; 
    else std::cout <<"had died"<<std::endl;
}

int main()
{
    auto p=std::make_unique<Cat>();
    //auto a = p; // ❌ 编译报错！unique_ptr 禁止拷贝
    //转移用std::move
    auto c=std::move(p);
    c->sound();
    if(p)p->sound();//p已经是空指针，调用是未定义行为
    check(p);
    check(c);
    return 0;
}

*/




