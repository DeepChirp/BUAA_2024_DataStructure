#include <stdio.h>

int main()
{
    int num, sum = 0, last_num = 0;
    char op, last_op = '+';

    while (scanf("%d %c", &num, &op) == 2)
    {
        switch (last_op)
        {
        case '+':
            sum += last_num;
            last_num = num;
            break;
        case '-':
            sum += last_num;
            last_num = -num;
            break;
        case '*':
            last_num *= num;
            break;
        case '/':
            last_num /= num;
            break;
        }
        if (op == '=')
            break;
        last_op = op;
    }
    sum += last_num;
    printf("%d\n", sum);

    return 0;
}