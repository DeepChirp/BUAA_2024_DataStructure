#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct Node
{
    char word[50];
    int count;
    struct Node *left, *right;
} Node;

Node *newNode(char *word)
{
    Node *node = (Node *)malloc(sizeof(Node));
    strcpy(node->word, word);
    node->count = 1;
    node->left = node->right = NULL;
    return node;
}

Node *insert(Node *node, char *word)
{
    if (node == NULL)
        return newNode(word);
    int cmp = strcmp(word, node->word);
    if (cmp < 0)
        node->left = insert(node->left, word);
    else if (cmp > 0)
        node->right = insert(node->right, word);
    else
        node->count++;
    return node;
}

void printTree(Node *node)
{
    if (node != NULL)
    {
        printTree(node->left);
        printf("%s %d\n", node->word, node->count);
        printTree(node->right);
    }
}

void printRoots(Node *node)
{
    if (node != NULL)
    {
        printf("%s", node->word);
        if (node->right != NULL)
        {
            printf(" %s", node->right->word);
            if (node->right->right != NULL)
            {
                printf(" %s", node->right->right->word);
            }
        }
        printf("\n");
    }
}

int main()
{
    FILE *file = fopen("article.txt", "r");
    char word[50];
    int index = 0;
    Node *root = NULL;
    int c;
    while ((c = fgetc(file)) != EOF)
    {
        if (isalpha(c))
        {
            word[index++] = tolower(c);
            if (index == 50)
            {
                word[--index] = '\0';
                root = insert(root, word);
                index = 0;
            }
        }
        else if (index > 0)
        {
            word[index] = '\0';
            root = insert(root, word);
            index = 0;
        }
    }
    if (index > 0)
    {
        word[index] = '\0';
        root = insert(root, word);
    }
    fclose(file);
    printRoots(root);
    printTree(root);
    return 0;
}