#include <stdio.h>
#include <stdlib.h>

struct Node
{
    int data;
    struct Node *left;
    struct Node *right;
};

struct Node *root = NULL;

struct Node *CreateNode(int data)
{
    struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));
    newNode->data = data;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

struct Node *InsertNode(struct Node *root, int data)
{
    if (root == NULL)
    {
        root = CreateNode(data);
    }
    else if (data < root->data)
    {
        root->left = InsertNode(root->left, data);
    }
    else
    {
        root->right = InsertNode(root->right, data);
    }
    return root;
}

void SearchLeaf(struct Node *root, int height)
{
    if (root == NULL)
    {
        return;
    }
    if (root->left == NULL && root->right == NULL)
    {
        printf("%d %d\n", root->data, height);
    }
    SearchLeaf(root->left, height + 1);
    SearchLeaf(root->right, height + 1);
}

int main()
{
    int n;
    int data;
    scanf("%d", &n);
    for (int i = 0; i < n; i++)
    {
        scanf("%d", &data);
        root = InsertNode(root, data);
    }
    SearchLeaf(root, 1);

    return 0;
}
