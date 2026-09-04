//抽象数据类型ADT
/*InitList(&L)       // 初始化空表
DestroyList(&L)    // 销毁表
ListLength(L)      // 返回表长
GetElem(L, i, &e)  // 取第 i 个元素存入 e
LocateElem(L, e)   // 查找第一个等于 e 的元素位置
ListInsert(&L, i, e) // 在第 i 位置插入元素 e
ListDelete(&L, i, &e) // 删除第 i 个元素并用 e 返回
TraverseList(L) */   // 遍历输出


//-----顺序表
/*
#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
#include <windows.h>
#endif

#define maxsize 100

typedef struct student
{
    int name[maxsize];
    int len;
} stu;

//初始化表
void init(stu *s)
{
    s->len=0;
}

//按值查询
int search(stu *s,int e)
{
    for(int i=0;i<s->len;i++)
    {
        if(s->name[i]==e)
        {
            printf("查找到%d\n",i+1);
            return 1;
        }
    }
    printf("没有找到\n");
    return 0;
}

//删除
int delete(stu *s,int i,int *e)
{
    if(i<1||i>s->len)//控制取值环境
    {
        printf("删除位置不对\n");
        return 0;
    }
    *e=s->name[i-1];
    for(int j=i-1;j<s->len-1;j++)//将元素前移
    {
      s->name[j]=s->name[j+1];
    }
    s->len--;
    return 1;
}

//插入
int insert(stu *s,int i,int e)
{
    if(i<1||i>s->len+1)//控制取值环境
    {
        printf("插入位置不对\n");
        return 0;
    }

    if(s->len>=maxsize)
    {
        printf("顺序表已经满了\n");
        return 0;
    }
    for(int j=s->len;j>=i;j--)//数组插入，必须要从后往前移动元素
    {
        s->name[j]=s->name[j-1];
    }
    s->name[i-1]=e;
    s->len++;
    return 1;
}

//打印表
int printlist(stu *s)
{
//检查表是否为空
    if(s->len==0)
    {
        printf("顺序表为空\n");
        return 0;
    }

    for(int i=0;i<s->len;i++)
    {
        printf("%d ",s->name[i]);
    }
    return 1;
}

int main()
{
#ifdef _WIN32
    // 将控制台输入/输出代码页设置为 UTF-8，避免中文乱码
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    stu s;
    init(&s);
    int n;
    printf("请输入初始学生成绩个数：");
    scanf("%d", &n);
    printf("请输入%d个成绩(整数):\n", n);
    for (int i = 0; i < n; i++) {
        int score;
        scanf("%d", &score);
        insert(&s, i+1, score);
    }
    printlist(&s);
}
*/


//-----单链表
/*
#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
#include <windows.h>
#endif

typedef struct LNode
{
    int data;
 struct LNode *next;
} LNode,*linklist;

//初始化单链表
linklist init()
{
    linklist L=(linklist)malloc(sizeof(LNode));//创建头节点
    if(L==NULL)//防止内存分配失败
    {
        printf("内存分配失败\n");
        return 0;
    }
    L->next=NULL;
    return L;
}

//头插法
void head_insert(linklist L,int n)//n为插入的个数
{
    printf("请输入%d个数据\n",n);
    for (int i=0;i<n;i++)
    {
        linklist p=(linklist)malloc(sizeof(LNode));
        scanf("%d",&p->data);

        p->next=L->next;
        L->next=p;
    }
        
}


//尾插法
void tail_insert(linklist L,int m)//m为插入的个数
{
    printf("输入%d个数据\n",m);
    linklist r=L;//r为尾指针，让L一直为头指针
    for (int i=0;i<m;i++)
    {
        linklist s=(linklist)malloc(sizeof(LNode));
        s->next=NULL;
        scanf("%d",&s->data);
        r->next=s;
        r=s;
    }
}

//遍历打印链表
void printlist(linklist L)
{
    linklist p=L->next;
    if(p==NULL)//检查链表是否为空
    {
        printf("链表为空\n");
        return;
    }
     
    while(p!=NULL)
    {
        printf("%d",p->data);
        p=p->next;
    }
}

//按位置插入
int insert(linklist L,int i,int e)//在第 i 个位置插入 e（i 从 1 开始计数）
{
    linklist p=L;
    int j=0;

    while(p!=NULL&&j<i-1)//范围是1~length+1，p->next可以为空
    {
        p=p->next;
        j++;
    }
    if (p == NULL || j > i - 1) return 0;  // 位置非法
    linklist s=(linklist)malloc(sizeof(LNode));
    s->data=e;
    s->next=p->next;
    p->next=s;
    return 1;
}

//按位置删除节点
int delete(linklist L,int i,int *e)//删除第i个元素，并且把值传给e
{
    linklist p=L;
    int j=0;
    while(p->next!=NULL&&j<i-1)//范围是1~length,p->next不能为空
    {
        p=p->next;
        j++;
    }

    if (p == NULL || j > i - 1) return 0;  // 位置非法

    linklist q=p->next;
    *e=q->data;//取值
    p->next=q->next;
    free(q);//释放内存，删除这个位置
    return 1;
}



//按值查找节点位置
int search(linklist L,int e)//查找第一个值为e的节点位置
{
    linklist p=L->next;
    int i=1;
    printf("查找的值为%d\n",e);

    while(p!=NULL)
    {
        if(p->data==e)
        {
            printf("查询到%d在第%d个位置\n",e,i);
            return 1;
        }
        p=p->next;
        i++;
    }
    printf("没有找到\n");
    return 0;
}


//删除链表
linklist destroy(linklist L)
{
    linklist p=L;
    while(p!=NULL)
    {
        linklist q=p->next;
        free(p);
        p=q;
    }
    return NULL;//让主函数的头指针不会变成野指针
}


int main(void) 
{
#ifdef _WIN32
    // 将控制台输入/输出代码页设置为 UTF-8，避免中文乱码
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif
    linklist L = init();
    int  n;

    printf("请输入初始元素个数：");
    scanf("%d", &n);
    tail_insert(L,n);
    printlist(L);
    return 0;
}
*/





//----双链表
/*
#include <stdio.h>
#include <stdlib.h>
#ifdef _WIN32
#include <windows.h>
#endif

typedef struct DNode
{
    int data;
    struct DNode *prior;// 指向前一个节点
    struct DNode *next;// 指向后一个节点
}DNode,*dlinklist;


//初始化
dlinklist init()
{
    dlinklist L=(dlinklist)malloc(sizeof(DNode));
    if(L==NULL)
    {
        printf("内存分配失败\n");
        return 0;
    }
    L->prior=NULL;
    L->next=NULL;
    return L;
}



//双链表尾插法
int tail_insert(dlinklist L,int n)
{
    dlinklist r=L;

    printf("输入数字\n");
    for(int i=0;i<n;i++)
    {
        dlinklist s=(dlinklist)malloc(sizeof(DNode));
        scanf("%d",&s->data);
        s->next=NULL;
        s->prior=r;
        r->next=s;
        r=s;
    }
}


//遍历打印
void printlist(dlinklist L)
{
    dlinklist p=L->next;
    if(p==NULL)
    {
        printf("链表为空表\n");
        return;
    }
    printf("当前链表\n");
    while(p!=NULL)
    {
        printf("%d ",p->data);
        p=p->next;
    }
    printf("\n");
}



//按位置插入节点
int listinsert(dlinklist L,int i,int e)
{
    dlinklist p=L;
    int j=0;
    while(p!=NULL&&j<i-1)
    {
        p=p->next;
        j++;
    }
    dlinklist s=(dlinklist)malloc(sizeof(DNode));
    s->data=e;
    s->next=p->next;
    s->prior=p;
    if (p->next != NULL) {    // 如果 p 不是最后一个节点
        p->next->prior = s;   // 原来 p 后继的前驱 = s
    }
    p->next=s;
    return 1;
}


//按位置删除节点
int listdelete(dlinklist L,int i,int *e)
{
    dlinklist p=L;
    int j=0;
    while(p!=NULL&&j<i-1)
    {
        p=p->next;
        j++;
    }
    dlinklist q=p->next;
    *e=q->data;

    p->next=q->next;
    if(p->next!=NULL)
    {
        q->next->prior=p;
    }
    free(q);
}


//修改指定位置的值
int change(dlinklist L,int i,int newvalue)
{
    dlinklist p=L;
    int j=0;
    while(p->next!=NULL&&j<i)
    {
        p=p->next;
        j++;
    }
    if(p->next==NULL)  return 0;
    p->data=newvalue;
    return 1;
}


int main()
{
    #ifdef _WIN32
    // 将控制台输入/输出代码页设置为 UTF-8，避免中文乱码
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif
    dlinklist L=init();
    int n,number;
    printf("你要输入的数字的个数\n");
    scanf("%d",&n);

    tail_insert(L,n);
    printlist(L);
}
*/

