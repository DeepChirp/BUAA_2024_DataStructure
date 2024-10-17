#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100
int graph[MAX][MAX] = {{0}}; // use adjacency matrix to store the graph
int visited[MAX] = {0};      // use visited array to store the visited vertex

void depthFirstSearch(int vertex);
void breadthFirstSearch(int vertex);

// search the graph by DFS
void depthFirstSearch(int vertex)
{
    visited[vertex] = 1;
    printf("%d ", vertex);
    for (int i = 0; i < MAX; i++)
    {
        if (graph[vertex][i] && !visited[i])
        {
            depthFirstSearch(i);
        }
    }
}

// search the graph by BFS
void breadthFirstSearch(int vertex)
{
    int queue[MAX] = {0};
    int front = 0, rear = 0;
    queue[rear++] = vertex;
    visited[vertex] = 1;
    while (front < rear)
    {
        int currentVertex = queue[front++];
        printf("%d ", currentVertex);
        for (int i = 0; i < MAX; i++)
        {
            if (graph[currentVertex][i] && !visited[i])
            {
                queue[rear++] = i;
                visited[i] = 1;
            }
        }
    }
}

int main()
{
    int vertex, edge;
    scanf("%d %d", &vertex, &edge);
    for (int i = 0; i < edge; i++)
    {
        int vertex1, vertex2;
        scanf("%d %d", &vertex1, &vertex2);
        graph[vertex1][vertex2] = 1;
        graph[vertex2][vertex1] = 1;
    }
    depthFirstSearch(0);
    putchar('\n');
    memset(visited, 0, sizeof(visited));
    breadthFirstSearch(0);
    putchar('\n');
    memset(visited, 0, sizeof(visited));

    int deleteVertex;
    scanf("%d", &deleteVertex);
    for (int i = 0; i < vertex; i++)
    {
        graph[deleteVertex][i] = 0;
        graph[i][deleteVertex] = 0;
    }
    depthFirstSearch(0);
    putchar('\n');
    memset(visited, 0, sizeof(visited));
    breadthFirstSearch(0);
    putchar('\n');
    memset(visited, 0, sizeof(visited));

    return 0;
}
