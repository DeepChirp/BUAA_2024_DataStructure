#include <stdio.h>
#include <math.h>

int main()
{
    int n;
    scanf("%d", &n);

    for (int number = n; number > 1; number--)
    {
        if (2 * n + number - number * number > 0 && (2 * n + number - number * number) % (2 * number) == 0)
        {
            int starter = (2 * n + number - number * number) / (2 * number);
            printf("%d=", n);
            for (int i = 0; i < number; i++)
            {
                printf("%d", starter + i);
                if (i != number - 1)
                {
                    printf("+");
                }
            }
            printf("\n");
            return 0;
        }
    }
    printf("No Answer\n");

    return 0;
}
