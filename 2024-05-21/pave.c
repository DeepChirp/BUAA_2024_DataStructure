#include <stdio.h>
#include <stdlib.h>

#define MAX 100

struct edge
{
    int id;
    int vertex1;
    int vertex2;
    int weight;
};

int parent[MAX];
int edgeIds[MAX];

int find(int x);
void unionSet(int x, int y);
int compareEdges(const void *a, const void *b);
int compareIds(const void *a, const void *b);

int find(int x)
{
    if (parent[x] != x)
    {
        parent[x] = find(parent[x]);
    }
    return parent[x];
}

void unionSet(int x, int y)
{
    parent[find(x)] = find(y);
}

int compareEdges(const void *a, const void *b)
{
    return ((struct edge *)a)->weight - ((struct edge *)b)->weight;
}

int compareIds(const void *a, const void *b)
{
    return (*(int *)a) - (*(int *)b);
}

int main()
{
    int vertex, edge;
    int sum = 0;
    int edgeCount = 0;
    scanf("%d %d", &vertex, &edge);
    struct edge edges[MAX];
    for (int i = 0; i < edge; i++)
    {
        scanf("%d %d %d %d", &edges[i].id, &edges[i].vertex1, &edges[i].vertex2, &edges[i].weight);
    }
    qsort(edges, (unsigned int)edge, sizeof(struct edge), compareEdges);
    for (int i = 0; i < vertex; i++)
    {
        parent[i] = i;
    }
    for (int i = 0; i < edge; i++)
    {
        if (find(edges[i].vertex1) != find(edges[i].vertex2))
        {
            unionSet(edges[i].vertex1, edges[i].vertex2);
            sum += edges[i].weight;
            edgeIds[edgeCount++] = edges[i].id;
        }
    }
    qsort(edgeIds, (unsigned int)edgeCount, sizeof(int), compareIds);
    printf("%d\n", sum);
    for (int i = 0; i < edgeCount; i++)
    {
        printf("%d ", edgeIds[i]);
    }

    return 0;
}