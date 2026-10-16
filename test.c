//第 1 题
//题目描述：编写程序，统计圆半径为1~10的整数（1、2、3......10）
//且圆面积小于100的所有的圆面积并输出

/*#include <stdio.h>
#include <windows.h>
#define PI 3.1415926

int main()
{
    SetConsoleOutputCP(65001);//设置为UTF-8编码，避免中文乱码

    int i;
    int r;
    double area;

    for(i=1;i<=10;i++)
    {
        r=i;
        area=PI*r*r;
        if(area<100)
        {
            printf("面积为%.2f\n",area);
        }
    }

    return 0;
}*/

//第 2 题 
//题目描述：输入一行长度不超过 100 的英文字符串，将其中所有小写
//英文字母转换为大写，大写英文字母和其他字符保持不变。
//输出转换完成后的字符串。 
//样例输入 
//Hello World! 
//样例输出 
//HELLO WORLD!



/*#include <stdio.h>
#include <windows.h>


int main()
{
    SetConsoleOutputCP(65001);

    char str[100];
    printf("请输入英文字符串");
    fgets(str, sizeof(str),stdin);
    int len=strlen(str);
    if(str[len-1]=='\n')
    {
        str[len-1]='\0';
    }

    
    for(int i=0;i<len;i++)
    {
        if(str[i]>='a'&&str[i]<='z')
        {
            str[i]=str[i]-'a'+'A';
        }
        printf("%c",str[i]);
        
    }
    return 0;
}*/


//第 3 题 
//题目描述：超市录入了 10 款的毛绒玩具的单价（整数，单位：元），存入一维数组。 
//请使用排序算法（冒泡排序法或选择排序法），将价格按从低到高升序排列，输出排序后的所有价格，数字之间用空格分隔。 
//要求：手动实现选择排序核心逻辑，禁止调用系统内置排序函数。 
//样例输入 
//12 5 28 95 15 22 7 19 109 2
//样例输出 
//2 5 7 12 15 19 22 28 95 109 


/*#include <stdio.h>
#include <windows.h>

void bubblue(int arr[], int n);

int main()
{
    int arr[] = {12,5,28,95,15,22,7,19,109,2 };
    int n = sizeof(arr) / sizeof(arr[0]);
    
    bubblue(arr, n);
    
    printf("排序结果\n");
    for (int i = 0;i < n;i++)
    {
        printf("%d",arr[i]);
        printf(" ");
    }

}

void bubblue(int arr[],int n)
{
    for(int i=0;i<n-1;i++)
    {
        for(int j=0;j<n-1-i;j++)
        {
            if(arr[j]>arr[j+1])
            {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}*/


//第 4题
//题目描述：菲波那契数列的定义为数列前两个数都是1，从第3个数开始，每个数都是前面两个数的和，
//编写函数实现调用该函数，输出该数列的第n项的数值，用递归法实现。
//样例输入
//请输入一个正整数：8
//样例输出
//菲波那契数列的第八项为：21

/*#include <stdio.h>
#include <windows.h>
int factrial(int x);


int main()
{
    SetConsoleOutputCP(65001);
    char arr[10];
    int x;
    printf("输入一个数字\n");
    
    fgets(arr, sizeof(arr), stdin);// 1. 先把整行读进缓冲区
    sscanf(arr, "%d", &x);// 2. 再从缓冲区里把整数解析出来

    printf("%d", factrial(x));
    return 0;

}

int factrial(int x)
{
    if (x == 1 || x == 2) { return 1; }
    else { return factrial(x - 1) + factrial(x - 2); }
}
*/



//第 5 题 
//题目描述：编写程序实现从一个3行4列的二维数组中找出最大数所在的行和列，
//并将最大值及所在行列值打印出来。二维数组的输入在主函数中进行，
//要求将查找功能编一个函数，并将二维数组通过指针参数传递的方式由主函数传递
//到子函数中，写出完整程序。

/*#include <stdio.h>
#include <windows.h>

void findmax(int (*arr)[4]);

int main()
{
    SetConsoleOutputCP(65001);
    int arr[3][4]={
        {1,2,3,4},
        {5,6,7,8},
        {9,10,11,12}
    };
    findmax(arr);
    return 0;
}

void findmax(int (*arr)[4])
{
    int i,j;
    int max=arr[0][0];
    int row=0,col=0;

    for(i=0;i<3;i++)
    {
        for(j=0;j<4;j++)
        {
            if (arr[i][j] > max)
            {
                max = arr[i][j];
                row=i;
                col=j;
            }
        }
    }
    printf("最大值为%d\n",max);
    printf("最大值所在行：%d\n",row);
    printf("最大值所在列：%d\n",col);
}
*/





//题目1：单链表反转（必会题）
//给定一个带头结点的单链表，将其原地反转。例如原链表：头 → 10 → 20 → 30 → NULL，反转后：头 → 30 → 20 → 10 → NULL。
//要求：空间复杂度 O(1)，不能使用额外数组。

/*
#include <stdio.h>
#include <stdlib.h>

typedef struct LNode
{
    int data;
    struct LNode *next;
}LNode, *linkstack;



linkstack init()
{
    linkstack p=(linkstack)malloc(sizeof(LNode));
    if(p==NULL)
    {
        printf("内存分配失败\n");
        return 0;
    }
    p->next=NULL;
    return p;
}


int tail(linkstack p,int n)
{
    linkstack r=p;
    printf("输入%d",n);
    for(int i=0;i<n;i++)
    {
        linkstack s=(linkstack)malloc(sizeof(LNode));
        scanf("%d",&s->data);
        s->next=NULL;
        r->next=NULL;
        r=s;
    }
    return 1;
}
int printlink(linkstack p)
{
    linkstack s = p->next;
    if (s == NULL) {
        printf("链表为空\n");
        return 0;
    }
    while (s != NULL) {
        printf("%d ", s->data);
        s = s->next;
    }
    printf("\n");
}

int ReverseList(linkstack L)
{
    linkstack p=L->next;
    L->next=NULL;
    while(p!=NULL)
    {
        linkstack q=p->next;
        p->next = L->next;     // 头插法：p 的 next 指向新链表首元
        L->next = p;           // 头结点指向 p，p 成为新首元
        p = q; 
        
    }

}
*/



//题目2：括号匹配（栈应用，必会题）
//编写函数 int isValid(char *s)，判断只包含小括号 ( 和 ) 的字符串是否合法。
//合法：(()())、()()
//不合法：)(、(()、())



//反转链表
//示例
/*
1→2→3→4→5 → 5→4→3→2→1，返回值为 5 的节点
空链表 → 返回 NULL
单节点 1 → 返回它本身
数据范围：节点数 [0, 5000]。

要求

用迭代写法：只用 prev、cur、next 三个指针变量，不许开数组
写成完整可编译运行的程序：节点定义、反转函数、main 函数齐全，测试链表在 main 里自己 malloc 串出来
至少 3 组测试：5 节点常规、单节点、空链表
写完要能回答这个问题：为什么必须先把 cur->next 存进 next，再改 cur->next 的指向？颠倒顺序会发生什么？
思考引导：想象链表被 cur 切成两截——左边是已经反转好的（挂在 prev 上），右边是还没动的。每轮循环只干一件事：
把 cur 摘下来接到左边头上。建议在纸上画 3 个节点一步步推。

*/

//我的代码
/*
#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
#include <windows.h>
#endif

#define maxsize 100

typedef struct listNode
{
    int num;
    struct listNode *next;
}listNode;

typedef struct SNode
{
    int data[maxsize];
    int top;
}stack;

listNode *init()
{
    listNode *head=(listNode*)malloc(sizeof(listNode));
    if(head==NULL) 
    {
        printf("内存分配失败\n");
        exit(1);
    }

    head->next=NULL;

    return head;
}

void stackinit(stack *s)
{
    s->top=-1;
}

int push(stack *s,int e)
{
    if(s->top==maxsize-1) 
    {
        printf("栈满\n");
        return 0;
    }
    s->top++;
    s->data[s->top]=e;
    return 1;
}

int pop(stack *s,int *e)
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

void printlist(listNode *p)
{
    listNode*s=p->next;
    while(s!=NULL)
    {
        printf("%d ",s->num);
        s=s->next;
    }
}

void reverselist(listNode *head,stack *tem)
{
    listNode *s=head->next;//用s代替p进行
    while(s!=NULL)
    {
        push(tem,s->num);
        s=s->next;

    }
    s=head->next;
    while(s != NULL && tem->top != -1)
    {
        int e;
        pop(tem,&e);
        s->num=e;
        s=s->next;
    }
}

void head_insert(listNode *p,int n)
{
    printf("请输入%d个数据\n",n);
    for(int i=0;i<n;i++)
    {
        listNode *q=(listNode*)malloc(sizeof(listNode));
        scanf("%d",&q->num);

        q->next=p->next;
        p->next=q;

    }
}

void freelist(listNode *head) {
    listNode *cur = head->next;
    while (cur != NULL) {
        listNode *temp = cur;
        cur = cur->next;
        free(temp);
    }
    free(head);   // 头节点也要释放
}

int main(void)
{
    #ifdef _WIN32
    // 将控制台输入/输出代码页设置为 UTF-8，避免中文乱码
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif
    listNode *p=init();
    stack s;
    stackinit(&s);
    int n;

    printf("输入个数\n");
    scanf("%d",&n);
    head_insert(p,n);
    

    printf("原本链表的样子\n");
    printlist(p);
    printf("反转以后的链表\n");
    reverselist(p,&s);
    printlist(p);
    freelist(p);
    return 0;
}
*/

//正确答案
#include <stdio.h>
#include <stdlib.h>

/* 链表节点定义 */
typedef struct ListNode {
    int val;
    struct ListNode *next;
}ListNode;

/* 反转链表：迭代写法 */
ListNode* reverseList( ListNode* head) {
    ListNode *prev = NULL;   // 已经反转好的那半截
    ListNode *cur  = head;   // 当前要处理的节点
    ListNode *next = NULL;   // 临时保存 cur 的下一个节点

    while (cur != NULL) {
        next = cur->next;   // ① 先记住下一个节点
        cur->next = prev;   // ② 把当前节点接到“已反转”的头上
        prev = cur;         // ③ prev 向前移
        cur = next;         // ④ cur 向前移
    }

    return prev;   // 最后 prev 就是新的头节点
}

/* 创建新节点 */
struct ListNode* createNode(int val) {
    struct ListNode* node = (struct ListNode*)malloc(sizeof(struct ListNode));
    node->val = val;
    node->next = NULL;
    return node;
}

/* 打印链表：1->2->3 */
void printList(struct ListNode* head) {
    while (head != NULL) {
        printf("%d", head->val);
        if (head->next != NULL) printf("->");
        head = head->next;
    }
    printf("\n");
}

/* 释放链表 */
void freeList(struct ListNode* head) {
    struct ListNode* cur = head;
    while (cur != NULL) {
        struct ListNode* temp = cur;
        cur = cur->next;
        free(temp);
    }
}

int main(void) {
    /* 测试 1：5 个节点 */
    struct ListNode* head1 = createNode(1);
    head1->next = createNode(2);
    head1->next->next = createNode(3);
    head1->next->next->next = createNode(4);
    head1->next->next->next->next = createNode(5);

    printf("Test1 before: "); printList(head1);
    head1 = reverseList(head1);
    printf("Test1 after : "); printList(head1);
    freeList(head1);

    /* 测试 2：单节点 */
    struct ListNode* head2 = createNode(1);
    printf("\nTest2 before: "); printList(head2);
    head2 = reverseList(head2);
    printf("Test2 after : "); printList(head2);
    freeList(head2);

    /* 测试 3：空链表 */
    struct ListNode* head3 = NULL;
    printf("\nTest3 before: (empty)\n");
    head3 = reverseList(head3);
    printf("Test3 after : "); printList(head3);

    return 0;
}






