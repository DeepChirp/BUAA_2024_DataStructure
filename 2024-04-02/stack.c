#include <stdio.h>

#define MAXSIZE 100

struct Stack
{
    int data[MAXSIZE];
    int top;
};

struct Stack s;

void Push(int num)
{
    if (s.top == MAXSIZE - 1)
    {
        printf("error ");
    }
    else
    {
        s.top++;
        s.data[s.top] = num;
    }
}

void Pop()
{
    if (s.top == -1)
    {
        printf("error ");
    }
    else
    {
        printf("%d ", s.data[s.top]);
        s.top--;
    }
}

int main()
{
    s.top = -1;
    while (1)
    {
        int act, num;
        scanf("%d", &act);
        switch (act)
        {
        case 1:
            scanf("%d", &num);
            Push(num);
            break;

        case 0:
            Pop();
            break;

        case -1:
            return 0;
        }
    }

    return 0;
}
