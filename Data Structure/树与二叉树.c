//树 二叉树

//完全二叉树用数组最省；一般二叉树多用链式
//顺序结构————只适合完全二叉树

//链式树
/*
#include <stdio.h>
#include <stdlib.h>


typedef struct BTNode
{
    int data;
    struct BTNode *left;
    struct BTNode *right; 
} BTNode;

*/

/*
//前序  根-左-右
void front(BTNode *p)
{
    if(p==NULL) return;
    printf("%d",p->data);// 空树/空子树：返回
    front(p->left);
    front(p->right);
}

//中序 左-根-右
void middle(BTNode *p)
{
    if(p==NULL) return;
    middle(p->left);
    printf("%d",p->data);
    middle(p->right);
}


//后序 左-右-根
void last(BTNode *p)
{
    if(p==NULL) return;
    last(p->left);
    last(p->right);
    printf("%d",p->data);
}
*/

/*
#include <stdio.h>
#include <stdlib.h>


//二叉树结构体
typedef struct BTNode
{
    int data;
    struct BTNode *left;
    struct BTNode *right; 
} BTNode;

typedef struct SNode
{
    BTNode *data;
    struct SNode *next;
}SNode;

typedef struct 
{
    SNode *top;
} stack;
void init(stack *p)
{
    p->top==NULL;
}

int empty(stack *p)
{
    return p->top==-1;
}

int push(stack *p,BTNode *s)
{
    SNode *r=(SNode *)malloc(sizeof(SNode));
    if (r == NULL) 
    { printf("分配内存失败\n"); exit(1); }
    r->data=s;
    r->next=p->top;
    p->top=r;
    return 1;
}

BTNode* pop(stack *p)
{

}


//DFS
//前序非递归
void PreOrderNR(BTNode *root)
{
    if(root==NULL) return;

    stack s;
    init(&s);
    push(&s,root);

    while(!empty(&s))
    {
        BTNode *p=pop(&s);
        printf("%d",p->data); //弹出栈顶节点

        if (p->right != NULL)        // 6. 先把右孩子压栈
            Push(&s, p->right);      //    (因为栈后进先出,右先压=右后出)
        if (p->left != NULL)         // 7. 再把左孩子压栈
            Push(&s, p->left);       //    (左后压=左先出→先访问左边)
    }                                 // 8. 循环直到栈空
    printf("\n");

}



//中序非递归

void inorder(BTNode *root)
{
    stack s;
    init(&s);
    BTNode *p=root;// 用 p 在树上"游走"

    while(p!=NULL||!empty(&s))// ← 大前提: 还有节点 或 栈里还有货
    {
        while(p!=NULL)
        {
            push(&s,p);
            p=p->left;
        }
        if(!empty(&s))
        {
            p=pop(&s);
            printf("%d",p->data);
            p=p->right;
        }
    }


}
*/








//BFS层序遍历
/*
#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
#include <windows.h>
#endif

#define maxsize 10

typedef struct BTNode
{
    int data;
    struct BTNode *left;
    struct BTNode *right; 
} BTNode;



typedef struct 
{
    BTNode *data[maxsize]; 
    int front;
    int rear;
}linkqueue;

BTNode *NewNode(int v) 
{
    BTNode *p = (BTNode*)malloc(sizeof(BTNode));
    if (p == NULL) { printf("内存分配失败\n"); exit(1); }
    p->data = v;
    p->left = NULL;
    p->right = NULL;
    return p;
}

void initqueue(linkqueue *p)
{
    p->front=p->rear=0;
}
int  empty(linkqueue *p)
{
    return p->front==p->rear;
}

void enqueue(linkqueue *p,BTNode *Q)// 出队：取回节点指针
{
    if(p->front==(p->rear+1)%maxsize) return;
    p->rear=(p->rear+1)%maxsize;
    p->data[p->rear]=Q;
}

BTNode *pop(linkqueue *p)
{
    if(empty(p)) return NULL;
    p->front=(p->front+1)%maxsize;
    return p->data[p->front];
}

void levelorder(BTNode *root)
{
    if(root==NULL) return;
    linkqueue q;
    initqueue(&q);
    enqueue(&q,root);

    while(!empty(&q))
    {
        BTNode*p=pop(&q);
        printf("%d",p->data);
        if (p->left != NULL)          // ⑤ 左孩子非空则入队
            enqueue(&q, p->left);
        if (p->right != NULL)         // ⑥ 右孩子非空则入队
            enqueue(&q, p->right);        
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
    // 手动搭建一棵示例树：
    //         1
    //        / \
    //       2   3
    //      / \   \
    //     4   5   6
    BTNode *root = NewNode(1);
    root->left  = NewNode(2);
    root->right = NewNode(3);
    root->left->left   = NewNode(4);
    root->left->right  = NewNode(5);
    root->right->right = NewNode(6);

    printf("层序遍历结果: ");
    levelorder(root);

    return 0;
}
*/




//递归求二叉树的深度、节点总数、叶子数
/*
#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
#include <windows.h>
#endif

#define maxsize 10

//二叉树节点
typedef struct BTNode
{
    int data;
    struct BTNode *left;
    struct BTNode *right;
}BTNode;

BTNode *new(int n)
{
    BTNode*p=(BTNode*)malloc(sizeof(BTNode));
    if(p==NULL)
    {
        printf("内存分配失败\n");
        exit(1);
    }
    p->data=n;
    p->left=NULL;
    p->right=NULL;
    return p;
}


//递归求二叉树的深度
int depth(BTNode *p)
{
    if(p==NULL) return 0;
    int left=depth(p->left);
    int right=depth(p->right);
    return (left >right ? left :right)+1;
}


//递归求节点总数
int nodecount(BTNode *p)
{
    if(p==NULL) return 0;
    return nodecount(p->left)+nodecount(p->right)+1;// 左+右+根自己
}
    


//递归求叶子数
int leafcount(BTNode *p)
{
    if(p==NULL) return 0;
    if(p->left==NULL&&p->right==NULL)
    return 1;
    return leafcount(p->left)+leafcount(p->right);
}


int main(void)
{
#ifdef _WIN32
    // 将控制台输入/输出代码页设置为 UTF-8，避免中文乱码
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif
    BTNode *root=new(1);
    root->left=new(2);
    root->right = new(3);
    root->left->left   = new(4);
    root->left->right  = new(5);
    root->right->right = new(6);

   printf("深度    : %d\n", depth(root));      // 期望 3
    printf("节点总数: %d\n", nodecount(root));  // 期望 6
    printf("叶子数  : %d\n", leafcount(root));  // 期望 3 (4,5,6)

    return 0;
}

*/








//二叉搜索树 BST
/*
#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
#include <windows.h>
#endif

typedef struct BTNode 
{
    int data;
    struct BTNode *left;
    struct BTNode *right;
}BTNode;

BTNode *new(int n)
{
    BTNode *p=(BTNode*)malloc(sizeof(BTNode));
    if(p==NULL)
    {
        printf("内存分配失败\n");
        exit(1);
    }
    p->data=n;
    p->left=NULL;
    p->right=NULL;
    return p;

}

//中序遍历
void inorder(BTNode *p)
{
    if (p==NULL) return ;
    inorder(p->left);
    printf("%d ",p->data);
    inorder(p->right);
}

//BST查找
BTNode *BSTsearch(BTNode*root,int key)
{
    while(root!=NULL)
    {
    if(key==root->data) return root;
    else if(key<root->data) root=root->left;
    else root=root->right;
    }

    return NULL;//没有就返回NULL
}


//BST插入
BTNode *BSTinsert(BTNode*root,int key)
{
    if(root==NULL) return new(key);
    if(key<root->data)
        root->left=BSTinsert(root->left,key);
    else if(key >root->data)
        root->right=BSTinsert(root->right,key);
        else  ;// key 已存在, 什么都不做(不插重复)
        return root;
}

//找最小值
BTNode *findmin(BTNode *p) {
    while (p && p->left) p = p->left;
    return p;
}

//BST的删除
BTNode *BSTdelete(BTNode *root,int key)
{
    if(root==NULL) return NULL;
    if(key<root->data) 
    root->left=BSTdelete(root->left,key);
    else if(key>root->data)
    root->right = BSTdelete(root->right, key);

    else// 找到要删的 root
    {
        if(root->left==NULL)
        {
            BTNode*x=root->right;
            free(root);
            return x;
        }
        else if(root->right==NULL)
        {
            BTNode*x=root->left;
            free(root);
            return x;
        }
    
//如果有俩个孩子 必须用"后继值覆盖 + 删后继"两步
    BTNode *q=findmin(root->right);
    root->data=q->data;//挪用这个数据，后面要把它删掉
    root->right = BSTdelete(root->right, q->data); // 删除这个后继

}
    return root;
}

int main(void)
{
    #ifdef _WIN32
    // 将控制台输入/输出代码页设置为 UTF-8，避免中文乱码
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

     BTNode *root = NULL;
    int arr[] = {8, 3, 10, 1, 6, 14, 4, 7};
    for (int i = 0; i < 8; i++)
        root = BSTinsert(root, arr[i]);

    printf("中序遍历(应递增): ");
    inorder(root); printf("\n");
 // 查找
    BTNode *f = BSTsearch(root, 6);
    printf("查找6: %s\n", f ? "找到" : "未找到");

    // 删除叶子
    root = BSTdelete(root, 1);
    printf("删除1后: "); inorder(root); printf("\n");
    // 期望: 3 4 6 7 8 10 14

    // 删除两个孩子节点
    root = BSTdelete(root, 3);
    printf("删除3后: "); inorder(root); printf("\n");
    // 期望: 4 6 7 8 10 14

    return 0;


}

*/