#include <stdio.h>

int main()
{
    int n, b;
    scanf("%d %d", &n, &b);

    if (n == 0)
    {
        printf("0\n");
        return 0;
    }

    int isNegative = n < 0;
    if (isNegative)
    {
        n = -n;
    }

    char s[100];
    int i = 0;
    while (n)
    {
        s[i] = n % b < 10 ? n % b + '0' : n % b - 10 + 'A';
        n /= b;
        i++;
    }

    if (isNegative)
    {
        printf("-");
    }

    for (int j = i - 1; j >= 0; j--)
    {
        printf("%c", s[j]);
    }

    return 0;
}