#include <stdio.h>
#include <string.h>

int main()
{
    char num[101];
    scanf("%s", num);

    int len = strlen(num);
    int dot_pos = -1, first_non_zero_pos = -1;
    for (int i = 0; i < len; i++)
    {
        if (num[i] == '.')
        {
            dot_pos = i;
        }
        else if (num[i] != '0' && first_non_zero_pos == -1)
        {
            first_non_zero_pos = i;
        }
    }

    int exponent = dot_pos - first_non_zero_pos; // 0.000123 -> 0.123 * 10^-3
    if (exponent > 0)
        exponent--;

    printf("%c", num[first_non_zero_pos]);
    if (first_non_zero_pos + 1 != len)
    {
        printf(".");
        for (int i = first_non_zero_pos + 1; i < len; i++)
        {
            if (i != dot_pos)
                printf("%c", num[i]);
        }
    }
    printf("e%d\n", exponent);

    return 0;
}