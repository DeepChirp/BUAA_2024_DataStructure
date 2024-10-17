#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAXSIZE 200 // 队列容量
#define THRESHOLD 7 // 窗口增加阈值
#define MAXSVR 5    // 最大服务窗口数
#define MINSVR 3    // 最小服务窗口数

typedef struct
{
    int id;    // 客户编号
    int wtime; // 等待时间
    int stime; // 服务所需时间
} CustType;

int Winnum = MINSVR; // 提供服务的窗口数
// 客户等待队列
static CustType Cqueue[MAXSIZE];
// 队头队尾指示器
static int Cfront = 0;
// 队尾
static int Crear = -1;
// 队中元素个数
static int Cnum = 0;
// 每组客户数
int person[100] = {0};

bool isEmpty()
{
    return Cnum == 0;
}

bool isFull()
{
    return Cnum == MAXSIZE;
}

int getCustnum()
{
    return Cnum;
}

void enCustqueue(CustType c)
{
    if (isFull())
    { /* 队满，插入失败 */
        printf("Full queue!");
        exit(-1);
    }
    else
    { /* 队未满，插入成功 */
        Crear = (Crear + 1) % MAXSIZE;
        Cqueue[Crear] = c;
        Cnum++;
    }
}

CustType deCustqueue()
{
    CustType c;
    if (isEmpty())
    {
        printf("Empty queue!");
        exit(-1);
    }
    else
    { /* 队非空，删除成功 */
        c = Cqueue[Cfront];
        Cnum--;
        Cfront = (Cfront + 1) % MAXSIZE;
        return c;
    }
}

void updateCustqueue() // 更新等待队列中客户等待时间
{
    int i;
    for (i = 0; i < Cnum; i++)
    {
        Cqueue[(Cfront + i) % MAXSIZE].wtime++;
    }
}

void arriveCust(int n)
{
    int i;
    static int count = 1;
    CustType c;
    for (i = 0; i < person[n]; i++)
    {
        c.id = count++;
        c.wtime = 0;
        scanf("%d", &c.stime);
        enCustqueue(c);
    }
    while ((getCustnum() / Winnum) >= THRESHOLD && Winnum < MAXSVR)
    {
        Winnum++; // 增加服务窗口
    }
}

int service()
{
    int i;
    CustType c;
    static int timer[MAXSVR] = {0}; // 每个窗口的计时器，不为0表示正在服务，不会接受新客户
    for (i = 0; i < Winnum; i++)
    {
        if (timer[i] > 0)
        {
            timer[i]--; // 如果窗口正在服务，计时器减1
        }
        else if (!isEmpty())
        {
            c = deCustqueue(); // 如果窗口空闲且队列不空，客户出队
            printf("%d : %d\n", c.id, c.wtime);
            timer[i] = c.stime - 1; // 开始计时，服务时间减1
        }
        else
        {
            return 0; // 等待队列为空且服务时间到，不会有新客户
        }
    }
    if ((getCustnum() / Winnum) < THRESHOLD && Winnum > MINSVR)
    {
        Winnum--;
    }
    return 1;
}

int main()
{
    int cycle = 0; // 模拟周期
    int group;     // 客户组数
    scanf("%d", &group);
    for (int i = 0; i < group; i++)
    {
        scanf("%d", &person[i + 1]);
    }
    while (1)
    {
        cycle++;
        updateCustqueue(); // 更新客户等待时间
        if (cycle <= group)
        {
            arriveCust(cycle); // 新客户加入队列
        }
        // 并根据需要调整服务窗口数量
        if (service() == 0 && cycle > group)
        {
            break; // 等待队列为空且服务时间到，不会有新客户
        }
    }
    return 0;
}