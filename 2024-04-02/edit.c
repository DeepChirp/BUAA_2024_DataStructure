#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 200
#define MAX_WORD 600

typedef struct
{
    char *words[MAX];
    int top;
} Stack;

typedef struct
{
    int op;
    int pos;
    char *str;
} Operation;

Stack s;

void Pop()
{
    if (s.top <= 0)
    {
        // printf("Stack must contain at least one element\n");
        return;
    }
    free(s.words[s.top]);
    s.words[s.top] = NULL;
    s.top--;
}

void Push(char *word)
{
    if (s.top == MAX - 1)
    {
        printf("Stack is full");
        return;
    }
    s.words[s.top++] = word;
}

void Insert(char *words, char *substr, int pos, int n)
{
    s.words[n] = malloc(MAX_WORD * sizeof(char));

    strncpy(s.words[n], words, pos);
    *(s.words[n] + pos) = '\0';
    strncat(s.words[n], substr, strlen(substr));
    // There is a hidden bug if write `strlen(words) - pos` because the strlen is unsigned and pos is signed, so the result will be unsigned.
    if ((int)strlen(words) > pos) // Avoid using strncat because it will append a null character at the end of the string.
    {
        strncat(s.words[n], words + pos, strlen(words) - pos);
        *(s.words[n] + strlen(words) + strlen(substr)) = '\0';
    }
}

void Delete(char *words, int pos, int len, int n)
{
    s.words[n] = malloc(MAX_WORD * sizeof(char));

    strncpy(s.words[n], words, pos);
    *(s.words[n] + pos) = '\0';
    if ((int)strlen(words) > pos + len) // Avoid using strncat because it will append a null character at the end of the string
    {
        strncat(s.words[n], words + pos + len, strlen(words) - pos - len);
        *(s.words[n] + strlen(words) - len) = '\0';
    }
}

int main()
{
    // read the original string
    char *original = malloc(MAX_WORD * sizeof(char));
    fgets(original, MAX_WORD, stdin);
    strstr(original, "\n")[0] = '\0'; // remove the newline character
    int n;
    scanf("%d", &n);
    s.top = n;
    s.words[n] = original;

    // read the previous operations
    int op, pos;
    char *str = malloc(MAX_WORD * sizeof(char));
    Operation *ops = malloc(n * sizeof(Operation));
    for (int i = 0; i < n; i++)
    {
        scanf("%d %d %s", &op, &pos, str);
        ops[i].op = op;
        ops[i].pos = pos;
        // ops[i].str = strdup(str); // Avoid using strdup because the OJ will warn `implicit declaration of function 'strdup'`
        ops[i].str = malloc(strlen(str) + 1); // Allocate memory for the string
        if (ops[i].str != NULL)               // Check if memory allocation was successful
        {
            strcpy(ops[i].str, str); // Copy the string
        }
    }
    while (n--)
    {
        if (ops[n].op == 1) // 1 means insert, but it is opposite because we are going to reverse the operations
        {
            Delete(s.words[n + 1], ops[n].pos, strlen(ops[n].str), n);
        }
        else if (ops[n].op == 2) // 2 means delete, but it is opposite because we are going to reverse the operations
        {
            Insert(s.words[n + 1], ops[n].str, ops[n].pos, n);
        }
    }
    free(ops);
    ops = NULL;

    // read the current operations
    int exit_flag = 0;
    while (1)
    {
        scanf("%d", &op);
        switch (op)
        {
        case 1: // insert
            scanf("%d %s", &pos, str);
            s.top++;
            Insert(s.words[s.top - 1], str, pos, s.top);
            break;
        case 2: // delete
            scanf("%d %d", &pos, &n);
            s.top++;
            Delete(s.words[s.top - 1], pos, n, s.top);
            break;
        case 3: // undo
            Pop();
            break;
        case -1: // exit the loop
            exit_flag = 1;
            break;
        default:
            break;
        }
        if (exit_flag)
        {
            break;
        }
    }

    // print the final string
    printf("%s", s.words[s.top]);

    return 0;
}
