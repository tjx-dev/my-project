//----队列（先进先出）
#include <stdio.h>
#include <stdlib.h>
#ifdef _WIN32
#include <windows.h>
#endif

#define maxsize 10

/*
typedef struct 
{
    int data[maxsize];
    int front;
    int rear;
} queue;

//初始化
int init(queue *p)
{
    p->front=-1;
    p->rear=-1;
}

//检查队空
int isempty(queue *p)
{
    return p->front==p->rear;
}

//检查队满
int isfull(queue *p)
{
    return p->rear=maxsize-1;
}

//入队
int enqueue(queue *p,int *e)
{
    if(!isfull(p)) return 0;
    p->rear++;
    p->data[p->rear]=*e;
    return 1;
}

//出队
int dequeue(queue *p,int *e)
{
    if(isempty(p)) return 0;
    p->front++;
    *e=p->data[p->front];
    return 1;
}

*/




//---循环队列
/*
typedef struct {
    int data[maxsize];
    int front;   // 队头前一个位置
    int rear;    // 队尾位置
} cirqueue;


//初始化
void init(cirqueue *q) {
    q->front = 0;
    q->rear = 0;
}

//判断空
int empty(cirqueue *p)
{
    return p->front==p->rear;
}


//判断满（要牺牲一个位置来和空队区分）
int full(cirqueue *p)
{
    return (p->rear+1)%maxsize==p->front;
}

//入队
int enqueue(cirqueue *p,int *e)
{
    if((p->rear+1)%maxsize==p->front) return 0;

    p->rear=(p->rear+1)%maxsize;//rear 循环+1
    p->data[p->rear]=*e;
    return 1;
}

//出队
int dequeue(cirqueue *p,int *e)
{
    if(empty(p)) return 0;

    p->front=(p->front+1)%maxsize;
    *e=p->data[p->front];
    return 1;
}

//元素个数
int queuelen(cirqueue *p)
{
    return (p->rear-p->front+maxsize)%maxsize;
}

int main()
{
#ifdef _WIN32
    // 将控制台输入/输出代码页设置为 UTF-8，避免中文乱码
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif
    cirqueue p;
    init(&p);

    int n,q;
    printf("请输入几个数字\n");
    scanf("%d",&n);
    for(int i=1;i<=n;i++)
    {
        printf("第%d次输入你要输入的数字\n",i);
        scanf("%d",&q);
        if(enqueue(&p, &q)) 
        printf("入队 %d 成功\n", q);
        else    
        printf("第 %d 个入队失败：队已满！\n", i);
    }
    printf("元素个数%d\n",queuelen(&p));
}
*/




//---链式队列
/*
typedef struct QNode
{
    int data;
    struct  QNode *next;
}QNode;

typedef struct
{
    QNode *front;//指向头结点
    QNode *rear;//指向尾节点
}linkqueue;

//初始化
void init(linkqueue *q)
{
    q->front=q->rear=(QNode*)malloc(sizeof(QNode));
    if(q->front==NULL)
    {
        printf("内存分配失败\n");
        exit(1);
    }
    q->front->next=NULL;
}


//判断空队
int empty(linkqueue *q)
{
    return q->front->next==NULL;
}


//入队（尾插法）
int enqueue(linkqueue *p,int *e)
{
    QNode *s=(QNode*)malloc(sizeof(QNode));
    if(s==NULL)
    {
        printf("内存分配失败\n");
        exit(1);
    }
    s->data=*e;
    s->next=NULL;
    p->rear->next=s;// 原尾节点指向新节点
    p->rear=s;// 更新尾指针
    return 1;
}

//出队(头删)
int dequeue(linkqueue *p,int *e)
{
    if(empty(p)) return 0;
    QNode *q=p->front->next;
    *e=q->data;
    p->front->next=q->next;
    if(q==p->rear)// ★特判：若出的是唯一一个节点
    p->front=p->rear;// 出队后队列空了，rear要回到头结点
    free(q);
    return 1;
}


//遍历
int printlink(linkqueue *p)
{
    QNode*s=p->front->next;
    if(s==NULL)
    {
        printf("空队\n");
        return 0;
    }
    while(s!=NULL)
    {
        printf("%d",s->data);
        s=s->next;
    }
    printf("\n");
    return 1;
}



int main()
{
#ifdef _WIN32
    // 将控制台输入/输出代码页设置为 UTF-8，避免中文乱码
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif
    linkqueue p;
    init(&p);
    int e;
    for(int i=1;i<4;i++)
    {
        enqueue(&p,&i);
    }
    printlink(&p);

    while(dequeue(&p,&e)) 
        printf("元素为%d\n",e);

    printlink(&p);

    return 0;
}

*/



//用队列层序遍历二叉树













