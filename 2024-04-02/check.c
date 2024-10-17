#include <stdio.h>

#define MAXSIZE 100

typedef struct Stack
{
    char bracket[MAXSIZE];
    int line[MAXSIZE];
    int top;
} stack;

stack s;

void Push(char c, int line)
{
    if (s.top == MAXSIZE - 1)
    {
        printf("Stack overflow\n");
    }
    else
    {
        s.top++;
        s.bracket[s.top] = c;
        s.line[s.top] = line;
    }
}

void Pop()
{
    if (s.top == -1)
    {
        printf("Stack is empty\n");
    }
    else
    {
        s.top--;
    }
}

int main()
{
    s.top = -1;

    // open the file in read mode
    FILE *fp = fopen("example.c", "r");
    if (fp == NULL)
    {
        perror("Can't open the file\n");
        return 1;
    }

    // read the file character by character
    char c, next_c;
    char result[2 * MAXSIZE];
    int line = 1;
    int num = 0;

    while ((c = fgetc(fp)) != EOF)
    {
        if (c == '\n')
        {
            line++;
        }
        // deal with strings and characters
        else if (c == '"')
        {
            while ((c = fgetc(fp)) != '"' && c != EOF)
                ;
        }
        else if (c == '\'')
        {
            while ((c = fgetc(fp)) != '\'' && c != EOF)
                ;
        }
        // deal with comments
        else if (c == '/')
        {
            next_c = fgetc(fp);
            if (next_c == '/') // single-line comment
            {
                while ((c = fgetc(fp)) != '\n' && c != EOF) // skip the comment line
                    ;
                line++;
            }
            else if (next_c == '*') // multi-line comment
            {
                while ((c = fgetc(fp)) != EOF)
                {
                    if (c == '\n')
                    {
                        line++;
                    }
                    if (c == '*')
                    {
                        next_c = fgetc(fp);
                        if (next_c == '/')
                        { // End of the comment
                            break;
                        }
                        else
                        {
                            fseek(fp, -1, SEEK_CUR); // Not the end of the comment, move the file pointer back
                        }
                    }
                }
            }
            else
            {
                fseek(fp, -1, SEEK_CUR); // Not a comment, move the file pointer back
            }
        }
        // deal with brackets
        else if (c == '(' || c == '{')
        {
            if (c == '{' && s.bracket[s.top] == '(')
            {
                printf("without maching '%c' at line %d\n", s.bracket[s.top], s.line[s.top]);
                return 1;
            }
            else
            {
                Push(c, line);
                result[num++] = c;
            }
        }
        else if (c == ')' || c == '}')
        {
            if (s.top == -1)
            {
                printf("without maching '%c' at line %d\n", c, line);
                return 1;
            }
            else
            {
                char top = s.bracket[s.top];
                if ((c == ')' && top == '(') || (c == '}' && top == '{'))
                {
                    Pop();
                    result[num++] = c;
                }
                else
                {
                    printf("without maching '%c' at line %d\n", c, line);
                    return 1;
                }
            }
        }
    }
    fclose(fp);

    if (s.top != -1)
    {
        printf("without maching '%c' at line %d\n", s.bracket[s.top], s.line[s.top]);
        return 1;
    }
    else
    {
        for (int i = 0; i < num; i++)
        {
            printf("%c", result[i]);
        }
    }

    return 0;
}
