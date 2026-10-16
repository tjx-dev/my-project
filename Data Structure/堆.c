//堆
//因为下标从 0 开始，一个下标为 i 的节点：
//父类（i-1）/2 左子树2i+1 右子树2i+2

#include <stdio.h>

#ifdef _WIN32
#include <windows.h>
#endif

//演示父子下标 + 判断是否为堆
/*
int isminhead(int a[],int n)
{
    for(int i=0;i<n;i++)
    {
        int left=2*i+1;
        int right=2*i+2;

        if(left<n&&a[i]>a[left]) return 0;
        if(right<n&&a[i]>a[right]) return 0;
    }
    return 1;
}


int main(void)
{
    #ifdef _WIN32
    // 将控制台输入/输出代码页设置为 UTF-8，避免中文乱码
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    int a[]={1,2,5,4,7,8,9};
    int p=sizeof(a)/sizeof(a[0]);

    printf("数组下标与值:\n");
    for (int i = 0; i < p; i++) {
        printf("下标 %d: 值 %d", i, a[i]);
        int parent = (i - 1) / 2;
        if (i > 0) printf("  (父下标 %d, 值 %d)", parent, a[parent]);
        printf("\n");
    }

    printf("\n是小顶堆吗? %s\n", isminhead(a, p) ? "是" : "否");
}

*/




//建堆 从底向上

/*
 * siftDown: 让数组 a 中下标 i 的节点"下沉"到正确位置(大顶堆)
 * n: 堆大小; i: 从哪个下标开始调整
 * 逻辑: 比较 i 与它左右孩子, 把最大的换到 i; 换了就继续下沉
 */

/*
void shiftdown(int a[],int n,int i)
{
    while(1)
    {
    int largest=i;// 先假定 i 是三者中最大的
    int left=2*i+1;
    int right=2*i+2;
    
    if(left<n&&a[left]>a[largest])
    largest=left;
    if(right<n&&a[right]>a[largest])
    largest=right;

    if(largest==i) break;// i 已是最大, 不用下沉, 结束

    // 否则交换 i 与 largest
    int temp=a[i];
    a[i]=a[largest];
    a[largest]=temp;

    i=largest;// 继续对新位置下沉
    }
}



void maxheap(int a[],int n)
{
    for(int i=(n-1-1)/2;i>=0;i--)// 从后往前
    {
        shiftdown(a,n,i);
    }
}

void print(int a[],int n)
{
    for(int i=0;i<n;i++)
    {
        printf("%d ",a[i]);
    }
}

int main(void)
{
#ifdef _WIN32
    // 将控制台输入/输出代码页设置为 UTF-8，避免中文乱码
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    int a[]={3, 9, 8, 5, 4, 1, 7, 6};
    int n=sizeof(a)/sizeof(a[0]);

    printf("原本数组\n");
    print(a,n);
    maxheap(a,n);
    printf("重新排序\n");
    print(a,n);

    return 0;

}

*/



//siftUp 插入、删除堆顶
/*s
#define maxsize 10 

void shiftup(int a[],int i)
{
    while(i>0)
    {
        int parent=(i-1)/2;
    
        if(a[i]>=a[parent])
        {
            int tmp=a[i];
            a[i]=a[parent];
            a[parent]=tmp;
        }
        i=parent;

        if(a[i]<a[parent]) break;
    
    }
}

void shiftdown(int a[],int n,int i)
{
    while(1)
    {
    int largest=i;// 先假定 i 是三者中最大的
    int left=2*i+1;
    int right=2*i+2;
    
    if(left<n&&a[left]>a[largest])
    largest=left;
    if(right<n&&a[right]>a[largest])
    largest=right;

    if(largest==i) break;// i 已是最大, 不用下沉, 结束

    // 否则交换 i 与 largest
    int temp=a[i];
    a[i]=a[largest];
    a[largest]=temp;

    i=largest;// 继续对新位置下沉
    }
}


//删除堆顶
void popheap(int a[],int *heap)
{
    int max=a[0];
    *heap=*heap-1;
    a[0]=a[*heap];
    shiftdown(a,*heap,0);
}

//插入
void insertheap(int a[],int *heap,int num)
{
    if(maxsize<*heap) return;
    
    a[*heap]=num;
    shiftup(a,*heap);
    (*heap)++;
}



int main(void) 
{
    #ifdef _WIN32
    // 将控制台输入/输出代码页设置为 UTF-8，避免中文乱码
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif
    int a[maxsize];
    int heap = 0;

    // 逐个插入: 6, 8, 5, 10, 3, 7
    int vals[] = {6, 8, 5, 10, 3, 7};
    for (int i = 0; i < 6; i++) {
        insertheap(a, &heap, vals[i]);
        printf("插入 %2d 后: ", vals[i]);
    }
    printf("序列为\n");
    for (int i = 0; i < 6; i++) 
    {
        printf("%d ",a[i]);
    }
}

*/




//堆排序-建大顶堆 + 反复把堆顶最大元素搬到当前末尾   升序

/*
void shiftdown(int a[],int n,int i)
{
    while(1)
    {
        int largest=i;
        int left=2*i+1;
        int right=2*i+2;

        if(left<n&&a[largest]<a[left]) 
        largest=left;
        if(right<n&&a[largest]<a[right])
        largest=right;

        if(largest==i)
        break;

        int temp=a[i];
        a[i]=a[largest];
        a[largest]=temp;

        i=largest;
    }
}

// 建大顶堆 (自底向上) 

void maxheap(int a[],int n)
{
    for(int i=(n-1-1)/2;i>=0;i--)
    {
        shiftdown(a,n,i);
    }
}


void heapsort(int a[],int n)
{
    maxheap(a,n);

    for(int i=n-1;i>0;i--)
    {
        int tmp=a[i];
        a[i]=a[0];
        a[0]=tmp;
        shiftdown(a,i,0);// 对新的堆顶下沉(end后面的已排好,不在堆内)
    }
}

void print(int a[],int n)
{
    for(int i=0;i<n;i++)
    {
        printf("%d ",a[i]);
    }
}



int main(void)
{    
#ifdef _WIN32
    // 将控制台输入/输出代码页设置为 UTF-8，避免中文乱码
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif
    int a[]={4, 10, 3, 5, 1, 8, 7, 2, 9, 6};
    int n = sizeof(a) / sizeof(a[0]);

    printf("排序前: "); print(a, n);
    heapsort(a, n);
    printf("排序后: "); print(a, n);   // 期望升序
    return 0;

}
*/




//Top K（小顶堆维护前 K 大）
/*
//小顶堆
void shiftdownmin(int a[],int n,int i)
{
     while (1) {
        int smallest = i;
        int l = 2 * i + 1, r = 2 * i + 2;
        if (l < n && a[l] < a[smallest]) smallest = l;
        if (r < n && a[r] < a[smallest]) smallest = r;
        if (smallest == i) break;
        int t = a[i]; a[i] = a[smallest]; a[smallest] = t;
        i = smallest;
    }
}

//topk
void topk(int a[],int n,int heap[],int K)
{
    int size=0;
    for(int i=0;i<n;i++)
    {
        if(size<K)
    {
        heap[size]=a[i];
        // 上浮到正确位置
        int j=size;

        while(j>0)
        {
            int p=(j-1)/2;
            if (heap[j] < heap[p]) 
            {
                int tmp = heap[j];
                heap[j] = heap[p];
                heap[p] = tmp;

                j = p;
            }else break;
        }
    
        size++;
    }else if (a[i] > heap[0]) 
        {   // ② 堆已满, 且新数据比堆顶(最小门槛)大
            heap[0] = a[i];            // 替换堆顶
            shiftdownmin(heap, K, 0);   // 下沉恢复小顶堆
        }
        // 否则: 新数据 <= 堆顶, 连前K都排不进, 丢弃
    }
}



int main(void) 
{
#ifdef _WIN32
    // 将控制台输入/输出代码页设置为 UTF-8，避免中文乱码
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif
    int a[] = {3, 8, 1, 9, 4, 7, 2, 10, 5, 6};
    int n = sizeof(a) / sizeof(a[0]);
    int K = 3;                    // 找前 3 大
    int heap[K];

    topk(a, n, heap, K);

    printf("数组中前 %d 大(无顺序): ", K);
    for (int i = 0; i < K; i++) printf("%d ", heap[i]);
    printf("\n");
    return 0;
}
*/
