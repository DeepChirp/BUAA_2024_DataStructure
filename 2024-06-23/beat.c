#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_SIZE 5000

struct device
{
    int type;
    int parent;
    int port;
    int child[33];
    bool isBeat;
};

struct device devices[MAX_SIZE];

void PreOrder(int id)
{
    if (id != 0 && !devices[id].isBeat)
    {
        printf("%d ", id);
        return;
    }
    for (int i = 1; i < 33; i++)
    {
        if (devices[id].child[i] != 0)
        {
            PreOrder(devices[id].child[i]);
        }
    }
}

int main()
{
    int sum;
    scanf("%d", &sum);
    for (int i = 0; i < sum; i++)
    {
        int id;
        scanf("%d", &id);
        scanf("%d %d %d", &devices[id].type, &devices[id].parent, &devices[id].port);
        if (devices[id].parent != -1)
        {
            devices[devices[id].parent].child[devices[id].port] = id;
        }
    }
    int n;
    while (scanf("%d", &n) != EOF && n != -1)
    {
        devices[n].isBeat = true;
        while (devices[n].parent != 0)
        {
            devices[devices[n].parent].isBeat = true;
            n = devices[n].parent;
        }
    }

    PreOrder(0);

    return 0;
}
