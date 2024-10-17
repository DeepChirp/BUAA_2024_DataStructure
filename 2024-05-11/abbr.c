#include <stdio.h>

int main()
{
    char c, prev;
    int count = 0;

    prev = getchar();
    putchar(prev);

    while ((c = getchar()) != EOF)
    {
        if (c - prev == 1)
        {
            count++;
        }
        else
        {
            switch (count)
            {
            case 0:
                break;
            case 1:
                putchar(prev);
                break;
            default: // count >= 2
                putchar('-');
                putchar(prev);
                break;
            }
            putchar(c);
            count = 0;
        }
        prev = c;
    }

    return 0;
}