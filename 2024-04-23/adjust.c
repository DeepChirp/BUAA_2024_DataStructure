#include <stdio.h>
#include <stdlib.h>

#define MAX 100

typedef struct Node
{
    int id;
    int flow;
} Node;

typedef struct TreeNode
{
    int id;
    int children[3];
} TreeNode;

typedef struct Queue
{
    TreeNode nodes[MAX];
    int front;
    int rear;
} Queue;

void enqueue(Queue *q, TreeNode node)
{
    q->nodes[q->rear++] = node;
}

TreeNode dequeue(Queue *q)
{
    return q->nodes[q->front++];
}

int isEmpty(Queue *q)
{
    return q->front == q->rear;
}

int Compare(const void *a, const void *b)
{
    Node *nodeA = (Node *)a;
    Node *nodeB = (Node *)b;
    if (nodeA->flow != nodeB->flow)
    {
        return nodeB->flow - nodeA->flow;
    }
    else
    {
        return nodeA->id - nodeB->id;
    }
}

void adjust(Node nodes[], TreeNode tree[], int n)
{
    // use BFS to adjust the tree
    Queue q = {.front = 0, .rear = 0};
    enqueue(&q, tree[0]);
    int i = 0;
    while (!isEmpty(&q) && i < n)
    {
        TreeNode node = dequeue(&q);
        for (int j = 0; j < 3 && node.children[j] != -1; j++)
        {
            if (node.children[j] < 100) // means gate, print
            {
                printf("%d->%d\n", nodes[i++].id, node.children[j]);
            }
            else // means node, enqueue
            {
                for (int k = 0; k < MAX; k++)
                {
                    if (tree[k].id == node.children[j])
                    {
                        enqueue(&q, tree[k]);
                        break;
                    }
                }
            }
        }
    }
}

int main()
{
    TreeNode tree[MAX];
    Node nodes[MAX];
    int n = 0, m = 0;
    int id, child;
    while (scanf("%d", &id) != EOF && id != -1)
    {
        tree[m].id = id;
        int i = 0;
        while (scanf("%d", &child) != EOF && child != -1)
        {
            tree[m].children[i] = child;
            i++;
        }
        while (i < 3)
        {
            tree[m].children[i] = -1; // -1 means no child
            i++;
        }
        m++;
    }
    while (scanf("%d %d", &id, &child) != EOF)
    {
        nodes[n].id = id;
        nodes[n].flow = child;
        n++;
    }
    qsort(nodes, n, sizeof(Node), Compare);
    adjust(nodes, tree, n);
    return 0;
}