#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define MAX_SIZE 200

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

typedef struct
{
    int top;
    struct Node *elements[MAX_SIZE];
} NodeStack;

typedef struct
{
    int top;
    char elements[MAX_SIZE];
} charStack;

typedef struct
{
    int top;
    int elements[MAX_SIZE];
} numStack;

charStack s1;
numStack s2;
NodeStack s3;

void pushChar(char element)
{
    if (s1.top < MAX_SIZE - 1)
    {
        s1.elements[++s1.top] = element;
    }
}

char popChar()
{
    if (s1.top >= 0)
    {
        return s1.elements[s1.top--];
    }
    return '\0';
}

int isEmptyChar()
{
    return s1.top == -1;
}

void pushNum(int element)
{
    if (s2.top < MAX_SIZE - 1)
    {
        s2.elements[++s2.top] = element;
    }
}

int popNum()
{
    if (s2.top >= 0)
    {
        return s2.elements[s2.top--];
    }
    return 0.0;
}

int isEmptyNum()
{
    return s2.top == -1;
}

void pushNode(struct Node *element)
{
    if (s3.top < MAX_SIZE - 1)
    {
        s3.elements[++s3.top] = element;
    }
}

struct Node *popNode()
{
    if (s3.top >= 0)
    {
        return s3.elements[s3.top--];
    }
    return NULL;
}

int precedence(char operator)
{
    switch (operator)
    {
    case '+':
    case '-':
        return 1;
    case '*':
    case '/':
        return 2;
    default:
        return 0;
    }
}

void remove_spaces(char str[])
{
    int count = 0;
    for (int i = 0; str[i]; i++)
    {
        if (str[i] != ' ')
        {
            str[count++] = str[i];
        }
    }
    str[count] = '\0';
}

void infix_to_postfix(char *infix, char *postfix)
{
    s1.top = -1;
    int i = 0, j = 0;
    int len = strlen(infix);
    while (i < len)
    {
        if (isdigit(infix[i]))
        {
            while (i < len && isdigit(infix[i]))
            {
                postfix[j++] = infix[i++];
            }
            postfix[j++] = ' '; // add a space to separate numbers
        }
        else if (infix[i] == '(')
        {
            pushChar(infix[i++]);
        }
        else if (infix[i] == ')')
        {
            while (!isEmptyChar() && s1.elements[s1.top] != '(')
            {
                postfix[j++] = popChar();
            }
            popChar();
            i++;
        }
        else
        {
            while (!isEmptyChar() && precedence(infix[i]) <= precedence(s1.elements[s1.top]))
            {
                postfix[j++] = popChar();
            }
            pushChar(infix[i++]);
        }
    }
    while (!isEmptyChar())
    {
        postfix[j++] = popChar();
    }
    strstr(postfix, "=")[0] = '\0';
}

int evaluate_postfix(char *postfix)
{
    s2.top = -1;
    s3.top = -1;
    int i;
    int len = strlen(postfix);
    for (i = 0; i < len; i++)
    {
        if (isdigit(postfix[i]))
        {
            int num = 0;
            while (isdigit(postfix[i]))
            {
                num = num * 10 + (postfix[i] - '0');
                i++;
            }
            pushNum(num);
            struct Node *ptr = CreateNode(num);
            pushNode(ptr);
        }
        else
        {
            int op2 = popNum();
            int op1 = popNum();
            struct Node *ptr1 = popNode();
            struct Node *ptr2 = popNode();
            struct Node *ptr = malloc(sizeof(struct Node));
            ptr->data = postfix[i];
            ptr->left = ptr2;
            ptr->right = ptr1;
            root = ptr;
            pushNode(ptr);
            switch (postfix[i])
            {
            case '+':
                pushNum(op1 + op2);
                break;
            case '-':
                pushNum(op1 - op2);
                break;
            case '*':
                pushNum(op1 * op2);
                break;
            case '/':
                pushNum(op1 / op2);
                break;
            default:
                break;
            }
        }
    }
    return popNum();
}

int main()
{
    char infix[MAX_SIZE], postfix[MAX_SIZE];
    fgets(infix, MAX_SIZE, stdin);
    remove_spaces(infix);
    infix_to_postfix(infix, postfix);
    int result = evaluate_postfix(postfix);
    printf("%c ", root->data);
    if (root->left->left != NULL)
    {
        printf("%c ", root->left->data);
    }
    else
    {
        printf("%d ", root->left->data);
    }
    if (root->right->left != NULL)
    {
        printf("%c \n", root->right->data);
    }
    else
    {
        printf("%d \n", root->right->data);
    }
    printf("%d\n", result);

    return 0;
}