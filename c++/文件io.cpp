//文件io

#include <iostream>
#include <fstream>
#include <string>

int main()
{
    std::ofstream fout("score.txt");

    fout <<"mjc 88\n";
    fout <<"tjx 98\n";

    fout.close();

    std::ifstream fin("score.txt");
    if(!fin) {std::cout <<"打不开\n"; return 1;};

    std::string name;
    int score ,sum=0,n=0;
    while (fin >> name >>score)
    {
        std::cout <<name <<" "<<score <<std::endl;

        sum+=score;
        n++;
    }
    std::cout<<(double)sum/n <<std::endl;    

    std::ofstream fapp("score.txt",std::ios::app);
    fapp <<"lzh 60\n";
    fapp.close();

    //重新统计，sum和n重新为0
    sum=0,n=0;
    fin.close();
    fin.open("score.txt");// ★ 重开：回到文件开头、状态复位
    if(!fin) {std::cout <<"打不开\n"; return 1;};
    
    while (fin >> name >>score)
    {
        std::cout <<name <<" "<<score <<std::endl;

        sum+=score;
        n++;
    }
    std::cout<<(double)sum/n <<std::endl;
    return 0;


}



