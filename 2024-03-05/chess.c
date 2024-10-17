#include <stdio.h>

int main()
{
    int chess[21][21];

    for (int i = 0; i < 21; i++)
    {
        for (int j = 0; j < 21; j++)
        {
            chess[i][j] = -1;
        }
    }

    for (int i = 1; i <= 19; i++)
    {
        for (int j = 1; j <= 19; j++)
        {
            scanf("%d", &chess[i][j]);
        }
    }

    for (int i = 1; i <= 19; i++)
    {
        for (int j = 1; j <= 19; j++)
        {
            if (chess[i][j] != 0)
            {
                int temp = chess[i][j];
                // 横行
                for (int k = 1; chess[i][j + k] == temp && j + k <= 19; k++)
                {
                    if (k == 3 && (chess[i][j + k + 1] == 0 || chess[i][j - 1] == 0))
                    {
                        printf("%d:%d,%d\n", temp, i, j);
                        return 0;
                    }
                }
                // 纵列
                for (int k = 1; chess[i + k][j] == temp && i + k <= 19; k++)
                {
                    if (k == 3 && (chess[i + k + 1][j] == 0 || chess[i - 1][j] == 0))
                    {
                        printf("%d:%d,%d\n", temp, i, j);
                        return 0;
                    }
                }
                // 右下斜
                for (int k = 1; chess[i + k][j + k] == temp && i + k <= 19 && j + k <= 19; k++)
                {
                    if (k == 3 && (chess[i + k + 1][j + k + 1] == 0 || chess[i - 1][j - 1] == 0))
                    {
                        printf("%d:%d,%d\n", temp, i, j);
                        return 0;
                    }
                }
                // 右上斜
                for (int k = 1; chess[i + k][j - k] == temp && i + k <= 19 && j - k >= 1; k++)
                {
                    if (k == 3 && (chess[i + k + 1][j - k - 1] == 0 || chess[i - 1][j + 1] == 0))
                    {
                        printf("%d:%d,%d\n", temp, i, j);
                        return 0;
                    }
                }
            }
        }
    }

    printf("No\n");

    return 0;
}
