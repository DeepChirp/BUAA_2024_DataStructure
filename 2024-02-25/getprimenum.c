#include <stdio.h>

int isPrime(int num)
{
    if (num < 2)
        return 0;
    for (int i = 2; i * i <= num; i++)
        if (num % i == 0)
            return 0;
    return 1;
}

int reverse(int num)
{
    int rev = 0;
    while (num)
    {
        rev = rev * 10 + num % 10;
        num /= 10;
    }
    return rev;
}

int main()
{
    int num;
    scanf("%d", &num);

    if (isPrime(num) && isPrime(reverse(num)))
        printf("yes\n");
    else
        printf("no\n");

    return 0;
}
