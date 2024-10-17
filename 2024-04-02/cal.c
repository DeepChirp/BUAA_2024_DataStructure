#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define MAX_SIZE 200

typedef struct
{
    int top;
    char elements[MAX_SIZE];
} charStack;

typedef struct
{
    int top;
    double elements[MAX_SIZE];
} numStack;

charStack s1;
numStack s2;

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

void pushNum(double element)
{
    if (s2.top < MAX_SIZE - 1)
    {
        s2.elements[++s2.top] = element;
    }
}

double popNum()
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

double evaluate_postfix(char *postfix)
{
    s2.top = -1;
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
        }
        else
        {
            double op2 = popNum();
            double op1 = popNum();
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
    printf("%.2f\n", evaluate_postfix(postfix));
    return 0;
}