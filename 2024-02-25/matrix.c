#include <stdio.h>

int main()
{
    int n;
    scanf("%d", &n);

    int matrix[n][n];

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            scanf("%d", &matrix[i][j]);
        }
    }
    getchar();
    char symbol;

    while ((symbol = getchar()) != '#')
    {
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                int temp;
                scanf("%d", &temp);
                if (symbol == '+')
                {
                    matrix[i][j] += temp;
                }
                else if (symbol == '-')
                {
                    matrix[i][j] -= temp;
                }
            }
        }
        getchar();
    }

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            printf("%5d", matrix[i][j]);
        }
        printf("\n");
    }

    return 0;
}
