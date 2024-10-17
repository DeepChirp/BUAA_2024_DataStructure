#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

struct memory
{
    int address;
    int size;
    struct memory *next;
};

struct memory *head = NULL;
struct memory *current = NULL;

void createMemory(int address, int size)
{
    struct memory *p = (struct memory *)malloc(sizeof(struct memory));
    p->address = address;
    p->size = size;
    p->next = NULL;
    if (head == NULL)
    {
        head = p;
        p->next = head;
        current = head;
    }
    else
    {
        struct memory *q = head;
        while (q->next != head)
        {
            q = q->next;
        }
        q->next = p;
        p->next = head;
    }
}

void deleteMemory(struct memory *p)
{
    if (p == head)
    {
        struct memory *q = head;
        while (q->next != head)
        {
            q = q->next;
        }
        head = head->next;
        q->next = head;
        current = head;
    }
    else
    {
        struct memory *q = head;
        while (q->next != p)
        {
            q = q->next;
        }
        q->next = p->next;
        if (p == current)
        {
            current = q->next;
        }
    }
    free(p);
}

int main()
{
    int n;
    scanf("%d", &n);
    for (int i = 0; i < n; i++)
    {
        int address, size;
        scanf("%d %d", &address, &size);
        createMemory(address, size);
    }

    int request;
    while (scanf("%d", &request) != EOF && request != -1)
    {
        struct memory *p = current;
        struct memory *q = NULL;
        int minus = INT_MAX;
        int flag = 0;
        do
        {
            if (p->size == request)
            {
                n--;
                current = p->next;
                deleteMemory(p);
                flag = 1;
                break;
            }
            else if (p->size > request && p->size - request < minus)
            {
                minus = p->size - request;
                q = p;
            }
            p = p->next;
        } while (p != current);
        if (flag == 0 && q != NULL)
        {
            q->size -= request;
            // update current
            current = q;
        }
    }

    // print
    struct memory *p = current;
    if (n > 0)
    {
        do
        {
            printf("%d %d\n", p->address, p->size);
            p = p->next;
        } while (p != current);
    }

    return 0;
}