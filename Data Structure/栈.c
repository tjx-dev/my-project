
#include <stdio.h>
#include <stdlib.h>

#define maxsize 100

#ifdef _WIN32
#include <windows.h>
#endif

//----顺序栈
/*
typedef struct
{
    int data[maxsize];
    int top;// 栈顶指针，指向栈顶元素的下标
} sqstack;


//初始化

void init (sqstack *s)
{
    s->top=-1;//空栈初始是-1
}


//判断是否为空栈
void stackempty(sqstack *s)
{
    if(s->top==-1)
    {
        printf("空栈\n");
        return ;
    }
}


//入栈
int push(sqstack *s,int e)
{
    if(s->top==maxsize-1)
    {
        printf("满栈\n");
        return 0;
    }
    s->top++;// 栈顶指针上移
    s->data[s->top]=e;//新元素放入栈顶
    return 1;
}


//出栈
int pop(sqstack *s,int *e)
{
    if(s->top==-1)
    {
        printf("空栈\n");
        return 0;
    }
    
    *e=s->data[s->top];
    s->top--;
    return 1;
}


//取栈顶元素（不删除）
int gettop(sqstack *s,int *e)
{
        if (s->top == -1) 
        {
            printf("栈为空，无栈顶元素\n");
            return 0;
        }
        *e=s->data[s->top];
        return 1;
}


//获取栈中元素个数
int stacklen(sqstack s)
{
    return s.top+1;
}


//遍历
void printstack(sqstack s)
{
    if (s.top == -1) 
    {
        printf("栈为空\n");
        return;
    }
        printf("栈中元素（从栈底到栈顶）：");
        for(int i=0;i<=s.top;i++)
        {
            printf("%d ",s.data[i]);
        }
        printf("\n");
}



int main(void)
{
#ifdef _WIN32
    // 将控制台输入/输出代码页设置为 UTF-8，避免中文乱码
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif
    sqstack s;
    init(&s);

    printf("入栈 10, 20, 30\n");
    push(&s, 10);
    push(&s, 20);
    push(&s, 30);
    printstack(s);

    int e;
    pop(&s,&e);
    printf("出栈元素：%d\n", e);
    return 0;
}
*/





//-----链式栈

/*
typedef struct SNode
{
    int data;
    struct SNode *next;
}SNode,*linkstack;


//初始化
linkstack init()
{
    linkstack s=(linkstack)malloc(sizeof(SNode));
    s->next=NULL;
    return s;
}


//检查是否为空表
void empty(linkstack s)
{
    if(s->next==NULL)
    {
        printf("空栈\n");
        return;
    }
}


//入栈
int push(linkstack s,int e)
{
    linkstack p=(linkstack)malloc(sizeof(SNode));
    if(p==NULL)
    {
        printf("内存分配失败\n");
        return 0;
    }
    p->data=e;
    p->next=s->next;
    s->next=p;
    return 1;
}


//出栈
int pop(linkstack s,int *e)
{
    linkstack p=s->next;
    if(s->next==NULL)
    {
        printf("空栈\n");
        return 0;
    }
    *e=p->data;
    s->next=p->next;
    free(p);
    return 1;
}



//取栈顶元素（不删除）
int gettop(linkstack s,int *e)
{
    if (s->next == NULL) 
    {
        printf("栈为空\n");
        return 0;   
    }
    *e=s->next->data;
    return 1;
}



//遍历打印栈
void printlink(linkstack s)
{
    if (s->next == NULL) 
    {
        printf("栈为空\n");
        return;
    }
    linkstack p=s->next;
    printf("栈中元素（从栈顶到栈底）：");
    while(p!=NULL)
    {
        printf("%d ",p->data);
        p=p->next;
    }
    printf("\n");
}


//销毁链栈
void destory(linkstack *s)
{
    linkstack p=*s;
    if(p!=NULL)
    {
        linkstack q=p->next;
        free(p);
        p=q;
    }
    *s=NULL;
}


int main(void)
{
#ifdef _WIN32
    // 将控制台输入/输出代码页设置为 UTF-8，避免中文乱码
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif
    linkstack s=init();
    printf("入栈\n");

    push(s,10);
    push(s,20);
    push(s,30);
    empty(s);
    
    int e;
    if(pop(s,&e))
    {
        printf("出栈%d\n",e);
    }
    printlink(s);
    return 0;
}

*/



//栈的应用
//1.括号匹配

typedef struct SNode
{
    char data[maxsize];
    int top;
}sqstack;


//初始化
    void init(sqstack *s) 
    {
        s->top = -1; 
    }


//检查是否为空栈
int empty(sqstack s)
{
    return s.top==-1;
}

int push(sqstack *s,char e)
{
    if(s->top==maxsize-1)
    {
        printf("满栈\n");
        return 0;
    }
    s->top++;
    s->data[s->top]=e;
    return 1;
}

int pop(sqstack *s,char *e)
{
    if(s->top==-1)
    {
        printf("空栈\n");
        return 0;
    }
    *e=s->data[s->top];
    s->top--;
    return 1;
}


int match(char left,char right)
{
    return (left == '(' && right == ')') ||
        (left == '[' && right == ']') ||
        (left == '{' && right == '}');
}


// 括号匹配主函数
int check(char *str)
{
    sqstack s;
    init(&s);
    for(int i=0;str[i] !='\0';i++)
    {
        char ma=str[i];
        if(ma =='('||ma == '[' || ma == '{')
        {
            
            push(&s,ma); // 左括号入栈
        }
        else if(ma== ')' || ma == ']' || ma == '}')
        {
            if(empty(s)) return 0;
            char ch;
            pop(&s,&ch);
        if(!match(ch,ma)) return 0;// 不匹配
        }
    }
        return empty(s);// 结束后栈空才完全匹配
}


int main()
{
#ifdef _WIN32
    // 将控制台输入/输出代码页设置为 UTF-8，避免中文乱码
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif
    char str[maxsize];
    printf("请输入字符串\n");
    scanf("%s",str);

    if(check(str)) 
    printf("匹配成功\n");

    else 
    printf("不匹配");
    return 0;

}
