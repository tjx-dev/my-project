#include <stdio.h>
#include <stdlib.h>//动态内存分配、随机数生成、程序退出、字符串转数字
#include <time.h>//当前时间、程序运行耗时，或者给随机数种子时
#include <math.h>//平方根、乘方、三角函数、绝对值（浮点数)
#include <stdbool.h>//bool
#include <string.h>//字符串数组

/*int main(void)
{*/
    /*int x,n=0;
    printf("请输入一个正整数\n");
    scanf("%d",&x);
    
    do{
        x/=10;
        n++;
    }while(x>0);

    printf("位数是%d",n);*/

    /*int number;

    srand(time(0));
    number=rand()%100+1;

    int count=0;
    int answer=0;
    printf("猜数字游戏，请你输入一个1-100之间的整数\n");

    do{
        printf("输入你猜的数字\n");
        scanf("%d", &answer);              
        count++;
        if(answer>number){
            printf("你输入的数字太大了\n");
        }else if(answer<number) {
            printf("你输入的数字太小了\n");
        }

    }while(answer!=number);

    printf("恭喜你答对了，使用了%d次\n",count);
    return 0;*/



    /*int count=0,answer;
    int sum=0;
    
    printf("请输入一组整数（输入-1结束)\n");
    scanf("%d",&answer);
    while(answer!=-1){
        sum+=answer;
        count++;     
        scanf("%d",&answer);
    }
    double sum1=sum/count;
    printf("平均数是%.1f\n",sum1);*/



    /*int number;
    int digit;
    int ret=0;

    printf("输入一个数，求它的逆序\n");
    scanf("%d",&number);

    while(number>0){
        digit=number%10;
        ret=ret*10+digit;
        number=number/10;

    }
    printf("最后的结果是%d",ret);*/


    /*int n;
    printf("输入一个数字，求他的阶乘\n");
    scanf("%d",&n);

    int i=0,sum=1;
    for (i=1;i<=n;i++) {
        sum*=i;
    }
    printf("%d的阶乘是%d",n,sum);

    int sum=1,i=1;
    while (i<=n){
        sum*=i;
        i++;
    }
    printf("结果是%d",sum);*/



    /*int x;
    for(x=1;x<=100;x++)
    {
        int isprime=1;

        int i;
        for(i=2;i<x;i++)
        {
            if(x%i==0)
            {
                isprime=0;
                break;
            }
        }
        if(isprime==1)
        {
            printf("%d\t",x);
        }
    }*/


    /*int one, two,five;
    int x;
    printf("输入一个数字\n");
    scanf("%d",&x);

    for (one=1;one<x*10;one++){
        for (two=1;two<x*10/2;two++){
            for(five=1;five<x*10/5;five++){

                if(one+two*2+five*5==x*10){
                    
                printf("%d个一角和%d个倆角和%d个五角一共是%d元\n",one,two,five,x);
                }
            }

        }

    }*/

    /*int x;
    printf("输入一个数\n");
    scanf("%d",&x);

    int n;
    int sign=1;
    double sum=0.0;
    for(n=1;n<=x;n++)
    {
        if(n%2==0)
        {
            sign=-sign;
        }
        sum+=sign*1.0/n;
    }
    printf("结果是%.5lf\n",sum);*/


    /*int x;
    printf("输入一个数字\n");
    scanf("%d",&x);

    int t;
    int mask=1;
    t=x;

    while (t>9)
    {
        t/=10;
        mask*=10;
    }

    printf("mask=%d\n",mask);

    int m;
    m=x;

    while(mask>0)
    {
        m/=mask;
        printf("结果是%d\n",m);
        m=x%mask;
        mask/=10;
    }*/


    /*int a,b;
    printf("输入俩个值\n");
    scanf("%d %d",&a,&b);

    int min;
    if(a<b)
    {
        min=a;
    }
    else
    {
        min=b;
    }
    printf("最小值是%d\n",min);

    int i;
    for(i=2;i<min;i++)
    {
        if(a%i==0)
        {
            if(b%i==0)
            {
                printf("gcd=%d\n",i);
            }
        }
    }*/



    /*int number;
    printf("输入一个数字\n");
    scanf("%d",&number);
    
    if(number<0)
    {
        printf("fu ");
        number=-number;
    }

    int mask=1;
    int t;
    t=number;

    while(t>9)
    {
        t/=10;
        mask*=10;
    }

    int a;
    while(mask>0){
        a=number/mask;
        switch(a)
        {
            case 0:
            printf("ling ");break;
            case 1:
            printf("yi ");break;
            case 2:
            printf("er ");break;
            case 3:
            printf("san ");break;
            case 4:
            printf("si ");break;
            case 5:
            printf("wu");break;
            case 6:
            printf("liu ");break;
            case 7:
            printf("qi ");break;
            case 8:
            printf("ba ");break;
            case 9:
            printf("jiu ");break;


        }
        if(mask>9)
        {
            printf(" ");
        }
        number%=mask;
        mask/=10;
    }*/


    /*int a,n;
    int S=0;
    printf("请输入0~9的数字和1~8的数字\n");
    scanf("%d %d",&a,&n);

    int i,t=0;
    for(i=1;i<=n;i++)
    {
        t=t*10+a;
        S+=t;
    }
    printf("结果=%d",S);*/

    /*printf("字节长度=%d\n",sizeof(int));
    printf("字节长度=%d\n",sizeof(char));
    printf("字节长度=%d\n",sizeof(short));
    printf("字节长度=%d\n",sizeof(long));
    printf("字节长度=%d\n",sizeof(long long));*/

    /*printf("unsigned int 的范围是0~%u\n",-1);*/

    /*double ff=1e+5;
    double zz=1E+5;
    printf("科学计数法用e或者E%e %E %.1f",ff,zz,ff);*/


    /*printf("正无穷是%f\n",12.0/0.0);
    printf("负无穷是%f\n",-12.0/0.0);
    printf("不存在的%f\n",0.0/0.0);*/


    /*char a=1;
    char b='1';

    if(a==b)
    {
        printf("相同\n");
    }
    else
    {
        printf("不相同\n");
    }
    printf("%d %d",a,b);*/


    //类型转换
    /*printf("%d\n",(int)1.23);*/

    /*int t=2;
    int gcd(int a,int b)
    {
        if(a<0||b<0)
        {
            a=abs(a);
            b=abs(b);
        }

        while(b!=0)
        {
        t=a%b;
        a=b;
        b=t;
        }
        return a;
    }
    

    int main()
    {
        int result;
        result=gcd(-15,18);
        printf("最大公约数=%d\n",result);
    return 0;
}*/


    /*int isEven(int num)
    {
        if(num<0)
        {
            num=abs(num);
        }

        if(num%2==0)
        {
            printf("偶数");
            return 1;
        }
        else
        {
            printf("奇数");
            return 0;
        }

    }
       

    int main()
    {
        int result;
        result=isEven(-5);
        return 0;
        }*/


    //数组
    /*int arr[5];
    int i;
    printf("输入五个数字\n");

    for (i=0;i<=4;i++)
        {
            scanf("%d",&arr[i]);
        }

    for(i=4;i>=0;i--)
        {
            printf("倒序是=%d",arr[i]);
        }*/

        
    /*int arr[100];
    printf("输入数字求平均值\n");
    int count=0;
    int sum=0;
    double p;
    for(int i=0;i<100;i++)
    {
        scanf("%d",&arr[i]);
        if(arr[i]!=-1)
        {
        sum+=arr[i];
        count++;
        }else
        {
            break;
        }
    }

    if(count>0)
    {
        p=(double) sum/count;
            printf("平均数是=%f\n",p);
    }
    else
    {
        printf("没有输入有效数字\n");
    }*/


    /*const int number=10;
    int i;
    int count[number];
    for(i=0;i<number;i++)
    {
        count[i]=0;
    }

    int a;
    printf("输入0~9的数字\n");
    scanf("%d",&a);

    while(a!=-1)
    {
        if(a>=0&&a<=9)
        {
            count[a]++;
        }
        scanf("%d",&a);
    }
    for(int i=0;i<number;i++)
    {
        printf("%d %d\n",i,count[i]);
    }*/


    /*int a;
    printf("输入数字0~9(输入-1结束)\n");

    int sum=0;
    int count=0;
    while(1)
    {
        scanf("%d",&a);
        if(a==-1) break;
        sum+=a;
        count++;
    }

    if(count>0)
    {
        printf("平均数=%f",(double)sum/count);
    }
    else
    {
        printf("没有输入正确值");
    }*/

//tic-tia-toc游戏
    /*const int num=3;
    int arr[num][num];
    int result=-1;//-1没人赢 1赢 0赢

    for(int i=0;i<num;i++)
    {
        for(int j=0;j<num;j++)
        {
            scanf("%d",&arr[i][j]);
        }
    }*/


    /*const int num=3;
    int arr[3][3]={1, 2, 3, 4, 5, 6, 7, 8, 9};

    int sum=0;
    int sum1=0;
    for (int i=0;i<num;i++)
    {
        sum1+=arr[i][i];
        sum+=arr[i][num-1-i];
    }
    printf("对角线和=%d\n",sum+sum1-arr[1][1]);*/



    /*int i=0;
    printf("%p\n",&i);
    printf("%lu",sizeof(&i));*/




    /*void maxmin(int a[],int len,int *max,int *min);

    int main()
    {
        int max,min;
        int a[]={1,2,4,3,5,6,7,8,9,15,49,78,56,26,99,};
        maxmin(a,sizeof(a)/sizeof(a[0]),&max,&min);
        printf("max=%d min=%d\n",max,min);
    }

    void maxmin(int a[],int len,int *max,int *min)
    {
        *max=*min=a[0];
        for(int i=0;i<len;i++)
        {
            if(*min>a[i])
            {
                *min=a[i];
            }else if(*max<a[i])
            {
                *max=a[i];
            }

        }
    }*/


    //用返回值return是-1或者0，来判断是否成功进行
    /*int divide(int a,int b,int *result);

    int main(void)
    {
        int a=5,b=2;
        int result;
        if(divide(a,b,&result))
        {
            printf("除法成功，%d/%d=%d",a,b,result);
        }

    }
    
    int ret;
    int divide(int a,int b,int *result)
    {
        ret=1;
        if(b==0) 
        {
            ret=0;
        }
        else 
        {
            *result=a/b;
        }
        return ret;
    }*/



    /*int a[]={20,62,71,97,90};
    int *p=a;
    printf("%d\n",a[2]);
    printf("%p",a);*/



    /*int *a;
    int arr[5];
    printf("便利贴 a 的大小: %lu 字节\n", sizeof(a));   // 输出 8
    printf("整栋楼 arr 的大小: %lu 字节\n", sizeof(arr));*/  //输出20


    //malloc
    /*void *p;
    int cnt=0;
    while((p=malloc(100*1024*1024))){
        cnt++;
    }
    printf("%d",cnt);*/


    /*int *arr;
    int n=0;

    printf("你要多少个\n");
    scanf("%d",&n);

    arr=(int*)malloc(n*sizeof(int));
    if(arr==0)
    {
        printf("分配失败");
        return 1;
    }

    int i;
    for( i=0;i<n;i++)
    {
        arr[i]=i*10;
    }
    printf("%d",arr[1]);
    free(arr);
    arr=NULL;*/



    //数组倒序
    /*int main(void)
    {
        const int num=10;
        char arr[num];
        fgets(arr,num,stdin);

        int len=strlen(arr);
        if(arr[len-1]=='\n'){
            arr[len-1]='\0';
            len--;
        }

        char *str=arr+len-1;
        char *dest=arr;
        int i;
        int temp;

        for(i=0;i<len/2;i++)
        {
            temp=*dest;
            *dest=*str;
            *str=temp;
            dest++;
            str--;
        }
        printf("反转后%s\n",arr);
        return 0;
    }*/


    /*void printArray(int *arr, int len);

    int main(void)
{
    
    int a[] = {10,12,89,29,7};
    printArray(a,sizeof(a)/sizeof(a[0]));


}

    void printArray(int *arr, int len)
{
    int i;
    for(i=0;i<len;i++)
    {
        printf("%d\n",arr[i]);
    }
}*/



    //字符串
    /*char arr[]="hello";
    char *p=arr;
    while(*p!='\0')
    {
        printf("%c",*p);
        p++;
    }*/


    /*char *arr[12]={"Jan","Feb","Mar","Apr","May","Jun","Jul","Aus","Sep","Oct","Nov","Dec"};

    int i;
    for(i=0;i<12;i++)
    {
        printf("%d的月份 %s\n",i+1,arr[i]);
    }*/


    /*char s[10];
    printf("输入你的名字\n");
    fgets(s,sizeof(s),stdin);

    int i;
    for(i=0;i<sizeof(s);i++)
    {
        printf("%d",s[i]);
    }

    int len = strlen(s);
    if (s[len-1] == '\n') {
        s[len-1] = '\0';

    }
    printf("处理后的内容：%s", s); 
    return 0;*/


    /*putchar('h');//等于printf("%c",'h')
    putchar(65);
    
    int ch;
    ch=getchar(void);
    printf("%c",ch);
    printf("%d",ch);*/

    /*char str[]="world";
    char dest[20];
    strcpy(dest,str);//复制
    printf("%s\n",str);

    char str1[]="taijunxiang";
    char dest1[30]="majingcheng";
    strcat(dest1,str1);//拼接
    printf("%s\n",dest1);*/


     //拼接
    /*char*mycat(char *dest,char*str);

    int main(int *argc,char const*argv[])
    {
        char dest[]="majingcheng";
        char str[]="taijuniang";
        mycat(dest,str);
        return 0;
    }

    char*mycat(char*dest,char*str)
    {
        printf("%s%s",dest,str);
    }*/


    //倒序
    /*void reverse(char *str);

    int main()
    {
        char str[10];
        scanf("%9s",str);
        reverse(str);
    }

    void reverse(char *str)
    {
        int i;
        int num=strlen(str);
        for(i=0;i<num;i++)
        {
            printf("%c",str[num-i-1]);
        }
    }*/



    //数单词
    /*int countWords(char *str);

    int main(void)
    {
        int n;
        printf("输入你要多大的空间\n");
        scanf("%d",&n);//scanf()和fgets一起用的时候要有getchar（）吃掉回车
         getchar();


        char *str=(char*)malloc((n+1)*sizeof(char));

        if(str==NULL) 
        {
            printf("内存分配失败\n");
            return 1;
        }

        printf("输入你要数的单词\n");
        fgets(str,n+1,stdin);

        int len=strlen(str);
        if(len>0&&str[len-1]=='\n')
        {
            str[len-1]='\0';
        }
        countWords(str);

        free(str);
    }
    
    int countWords(char *str)
    {
        int count=0;
        int i;
        char *p;
        p=strchr(str,' ');//strchr是从左边开始找，strrchr是从右边开始找

    while(*p==' ')
        {
        p++;
        }
    if(*p=='\0')
    {
        return 0;
    }

    while ((p = strchr(p, ' ')) != NULL) 
    {
        count++;
        p++;
    }
        printf("%d",count+1);
    }
    

    //数字更换位置
    /*void rotate(char *arr, int len, int k);

    int main()
    {
        char arr[5]={1,2,3,4,5};
        rotate(arr,5,2);
    }

    void rotate(char *arr, int len, int k)
    {
        char ds[len];
        int i;

        for(int i = 0; i < len; i++) 
        {
        ds[(i + k) % len] = arr[i];
        }

        for(i=0;i<len;i++)  {
            printf("%d",ds[i]);
        }
    }*/
    

    //枚举
    //enum color{red,yellow,orange};元素第一个默认从0开始




    //结构体
    /*struct point 
    {
        int x;
        int y;
    };

    struct point getstruct();
    void outstruct(struct point p);

    int main()
    {
        struct point x={0,0};
        x=getstruct();
        outstruct(x);
    }

    struct point getstruct()
    {
        struct point p;
        scanf("%d",&p.x);
        scanf("%d",&p.y);
        printf("%d %d",p.x,p.y);
    }
    void outstruct(struct point p)
    {
        printf("%d %d",p.x,p.y);
    }*/

//计算时间
    /*struct time{
        int hour;
        int minutes;
        int seconds;
        };
    struct time gettime(struct time *tm);

    int main(void)
    {
        int i;
        struct time now[]={{11,59,59},{23,59,59},{12,0,0},{1,29,59},{19,25,46}};
        for(i=0;i< sizeof(now) / sizeof(now[0]);i++)
        {
            gettime(&now[i]);
            printf("%.2i %.2i %.2i",now[i].hour,now[i].minutes,now[i].seconds);
        }
    }

    struct time gettime(struct time *tm)
    {
        ++(*tm).seconds;
        if((*tm).seconds==60)
        {
           (*tm).seconds=0;
            ++(*tm).minutes;
        }
        if((*tm).minutes==60)
        {
            (*tm).minutes=0;
            ++(*tm).hour;
        }
        if((*tm).hour==24)
        {
           (*tm).hour=0;
        printf("新的一天\n");
        }
    }*/


//typedef
/*typedef struct time
    {
        int hour;
        int minute;
        int second;
    } TI;
    int main()
{
    TI p={12,45,48};
    printf("%d",p.hour);

}*/



//union 至少要装下最大的字节数， 必须是"所有成员中最大对齐要求"的整数倍
/*#include <stdio.h>

union Data {
    char c[13];   // 13 字节
    int i;        // 4 字节
    double d;     // 8 字节
};

int main() {
    union Data data;
    
    printf("sizeof(union Data) = %zu\n", sizeof(union Data));
    
    data.i = 10;
    data.d = 3.14;
    
    printf("data.i = %d\n", data.i);
    printf("data.d = %f\n", data.d);
    
    data.c[0] = 'A';
    data.c[1] = 'B';
    printf("data.i = %d\n", data.i);
    
    return 0;
}*/



/*char *f(void); 
int all=10;//全局变量

int main()
{
    f();
}

char *f()
{
    static int part=1;//静态本地变量
    part+=1;
    printf("%d",part);
}*/


//宏定义
/*#define PI 3.1415926

int main()
{
    printf("%d",2*PI);
    return 0;
}*/


//带参数的宏
/*#define f(x) (x*x)
int main()
{
    printf("%d",f(6));
}*/
/*#define min(a,b) ((a>b)?(b):(a))//a是否大于b，是则b，不是就是a
int main()
{
    printf("%d",min(8,10));
}*/


//预定义 __FILE__,__DATE__,__TIME__,__LINE__
/*#define pf printf
int main()
{
    pf("%s %d %s %s",__FILE__,__DATE__,__TIME__,__LINE__);
    return 0;
}*/



//文件
/*int main()
{
    printf("%09d\n",123);
    printf("%-d\n",-168);
    printf("%+d\n",123);
    //- 是左对齐，0 是用0填充，+ 可以在数字前面放-或者+
    printf("%.2f\n",123.0);
    printf("%*d\n",5,123);
    printf("%.*f\n",3,6788.0);
    //数字表示有几位数，*表示代替一个数字，有.就是小数，
    int num;
    scanf("%*d%d",&num);
    printf("%d",num);
    //*表示跳过，
}*/


//文件的标准模式
/*FILE*fp=fopen("文件名","读取方式");
    if(fp==null)
    {
        printf("文件打开失败！\b");
        return 1;
    }
fclose("文件名");*/



//冒泡排序
/*#include <windows.h>
void bubbleSort(int arr[], int n);

int main()
{
    int count=0;
    SetConsoleOutputCP(65001);
    int arr[100];
    int n = sizeof(arr) / sizeof(arr[0]);
    printf("输入你要输入的数字\n");
    for(int i=0;i<n;i++)
    {
    scanf("%d",&arr[i]);
    printf("\n");
    if(arr[i]==-1)
    {
        break;
    }
    count++;
    }

        bubbleSort(arr, count);
        for (int i=0;i<n;i++)
        {
            printf("%d", arr[i]);
        }
}

void bubbleSort(int arr[],int n)
{
    for(int i=0;i<n-1;i++)//次数
    {
        for(int j=0;j<n-1-i;j++)//缩小范围
        {
            if(arr[j]>arr[j+1])
            {
                int temp = arr[j];
                arr[j] = arr[j+1];
                arr[j + 1] = temp;
            }
        }
    }
}*/



//选择排序法
/*int main()
{
    int arr[] = { 7,3,1,5 };
    int n = sizeof(arr) / sizeof(arr[0]);

    for (int i = 0;i < n;i++) 
    {
        int j;
        int min = i;
        for ( j = i+1;j < n;j++)
        {
            if (arr[min] > arr[j])
            {
                min = j;
            }
        }
        if(min!=i)
        {
            int temp = arr[i];
            arr[i] = arr[min];
            arr[min] = temp;
        }
    }
    for(int i=0;i<n;i++)
    {
        printf("%d", arr[i]);
    }
}*/



//快速排序
/*#include <stdio.h>
#include <stdlib.h>
    // 比较函数：告诉 qsort 怎么比大小
int compare( const void *a,const void *b)
{
    // 先强制转成 int*，再取值相减
    int A = *(int*)a;
    int B = *(int*)b;
    return A - B;// 返回负数：a<b；正数：a>b
}
int main()
{
    int arr[] = { 4,7,2,9 };
    int n = sizeof(arr) / sizeof(arr[0]);

    // 参数：数组首地址，元素个数，单个元素大小，比较函数
    qsort(arr, n, sizeof(arr[0]), compare);
    for (int i = 0; i < n; i++) printf("%d ", arr[i]); 
    return 0;
}*/



//递归
/*#include <stdio.h>
#include <windows.h>
int fact(int x)
{
    if (x == 1) return 1;
    
    return x * fact(x - 1);
    
}

int main()
{
    SetConsoleOutputCP(65001);
    int result = fact(5);
    printf("5!=%d\n", result);
    return 0;
}*/


//动态内存
/*#include <stdio.h>
#include <stdlib.h>

const int num = 5;
// 1. malloc：申请 5 个 int（不初始化）
int* p1 = (int*)malloc(num * sizeof(int));
if (p1 == NULL) 
{
    printf("创建失败\n"); 
    return 1;
}
    p1[0] = 100;  // 手动赋值

    // 2. calloc：申请 5 个 int 并自动清零
    int* p2 = (int*)calloc(num, sizeof(int));
    if (p2 == NULL) { printf("失败"); return 1; }
    printf("calloc 默认值: %d\n", p2[0]); // 输出 0

    // 3. realloc：把 p1 从 5 个扩容到 10 个（可能会搬家）
    int* temp = (int*)realloc(p1, 10 * sizeof(int));
    if (temp == NULL) {
        // 扩容失败，原来的 p1 依然有效
        printf("扩容失败\n");
        free(p1);
        return 1;
    }
    p1 = temp;  // 更新指针（realloc 可能换了新地址）

    // 4. 用完必须释放！
    free(p1);  // 释放后 p1 变成野指针，建议置 NULL
    free(p2);
    p1 = NULL;
    p2 = NULL;
    return 0;
}*/


//位运算
/*#include <windows.h>

int main(){
    SetConsoleOutputCP(65001);
    int a = 5, b = 3;
    printf("%d\n", a & b);   // 结果是 1 相同为一，
    printf("%d\n", a | b);   // 结果是 7 有一则以一
    printf("%d\n", a ^ b);   // 结果是 6 相同为0
    printf("%d\n", ~a);
    return 0;
}*/
