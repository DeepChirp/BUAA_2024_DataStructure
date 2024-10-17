#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAXNAME 20
#define MAXFUNC 100
#define MAXSTACK 200

struct Function
{
    char name[MAXNAME];
    int callingFunctions[MAXFUNC];
    int callingCount;
};

struct Stack
{
    struct Function functions[MAXSTACK];
    int top;
};

struct Stack stack;

struct Function functions[MAXFUNC];
int functionCount = 0;

int FindFunction(char *name)
{
    for (int i = 0; i < functionCount; i++)
    {
        if (strcmp(functions[i].name, name) == 0)
        {
            return i;
        }
    }
    return -1;
}

void Push(struct Function function)
{
    stack.functions[++stack.top] = function;
}

void Pop()
{
    stack.top--;
}

int IsEmpty()
{
    return stack.top == -1;
}

void CreateFunction(char *name)
{
    struct Function function;
    strcpy(function.name, name);
    function.callingCount = 0;
    functions[functionCount++] = function;
}

int main()
{
    stack.top = -1;

    int op;
    char name[MAXNAME];
    while (scanf("%d", &op) != EOF)
    {
        if (op == 8)
        {
            scanf("%s", name);
            if (!IsEmpty()) // not the first function, record the call
            {
                int callerIndex = FindFunction(stack.functions[stack.top].name);
                int calleeIndex = FindFunction(name);
                if (calleeIndex == -1) // new function
                {
                    CreateFunction(name);
                    calleeIndex = functionCount - 1;
                }
                Push(functions[calleeIndex]);
                int i;
                for (i = 0; i < functions[callerIndex].callingCount; i++)
                {
                    if (functions[callerIndex].callingFunctions[i] == calleeIndex)
                    {
                        break;
                    }
                }
                if (i == functions[callerIndex].callingCount) // not found
                {
                    functions[callerIndex].callingFunctions[functions[callerIndex].callingCount++] = calleeIndex;
                }
            }
            else // first function
            {
                CreateFunction(name);
                Push(functions[functionCount - 1]);
            }
        }
        else if (op == 0)
        {
            Pop();
            if (stack.top == -1)
            {
                break;
            }
        }
    }

    for (int i = 0; i < functionCount; i++)
    {
        if (functions[i].callingCount > 0)
        {
            printf("%s:", functions[i].name);
            for (int j = 0; j < functions[i].callingCount; j++)
            {
                printf("%s", functions[functions[i].callingFunctions[j]].name);
                if (j < functions[i].callingCount - 1)
                {
                    printf(",");
                }
            }
            printf("\n");
        }
    }

    return 0;
}