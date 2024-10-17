#include <stdio.h>
#include <stdlib.h>

#define MAX 1000

struct route
{
    unsigned int id;
    unsigned int start;
    unsigned int end;
};

struct route routes[MAX];
int visited[MAX] = {0};

void searchFirstDepth(unsigned int vertex, unsigned int endVertex, unsigned int *route, int routeCount, unsigned int sum);

void searchFirstDepth(unsigned int vertex, unsigned int endVertex, unsigned int *route, int routeCount, unsigned int sum)
{
    visited[vertex] = 1;
    for (unsigned int i = 0; i < sum; i++)
    {
        if ((routes[i].start == vertex && !visited[routes[i].end]) || (routes[i].end == vertex && !visited[routes[i].start]))
        {
            route[routeCount] = routes[i].id;
            if (routes[i].end == endVertex)
            {
                for (int j = 0; j < routeCount + 1; j++)
                {
                    printf("%d ", route[j]);
                }
                putchar('\n');
            }
            else
            {
                unsigned int nextVertex = routes[i].start == vertex ? routes[i].end : routes[i].start;
                searchFirstDepth(nextVertex, endVertex, route, routeCount + 1, sum);
            }
        }
    }
    visited[vertex] = 0;
}

int main()
{
    unsigned int vertex, edge;
    scanf("%u %u", &vertex, &edge);
    for (unsigned int i = 0; i < edge; i++)
    {
        scanf("%u %u %u", &routes[i].id, &routes[i].start, &routes[i].end);
    }
    searchFirstDepth(0, vertex - 1, (unsigned int *)malloc(sizeof(unsigned int) * vertex), 0, edge);

    return 0;
}