#include <stdio.h>
#include <string.h>

// 去除字符串前的0
void remove_leading_zero(char *num)
{
    int len = strlen(num);
    int first_non_zero_pos = -1;
    for (int i = 0; i < len; i++)
    {
        if (num[i] != '0' && first_non_zero_pos == -1)
        {
            first_non_zero_pos = i;
        }
    }
    if (first_non_zero_pos == -1)
    {
        num[0] = '0';
        num[1] = '\0';
    }
    else
    {
        for (int i = 0; i < len - first_non_zero_pos; i++)
        {
            num[i] = num[i + first_non_zero_pos];
        }
        num[len - first_non_zero_pos] = '\0';
    }
}

int main()
{
    char num1[85], num2[85];
    scanf("%s%s", num1, num2);
    remove_leading_zero(num1);
    remove_leading_zero(num2);

    int len1 = strlen(num1), len2 = strlen(num2);
    int symbol = 0;
    if (len1 < len2 || (len1 == len2 && strcmp(num1, num2) < 0))
    {
        symbol = 1;
        char temp[85];
        strcpy(temp, num1);
        strcpy(num1, num2);
        strcpy(num2, temp);
        int temp_len = len1;
        len1 = len2;
        len2 = temp_len;
    }

    int carry = 0;
    for (int i = 0; i < len1; i++)
    {
        int digit1 = num1[len1 - i - 1] - '0';
        int digit2 = i < len2 ? num2[len2 - i - 1] - '0' : 0;
        int diff = digit1 - digit2 - carry;
        if (diff < 0)
        {
            diff += 10;
            carry = 1;
        }
        else
        {
            carry = 0;
        }
        num1[len1 - i - 1] = diff + '0';
    }

    remove_leading_zero(num1);
    if (symbol == 1)
    {
        printf("-");
    }

    printf("%s\n", num1);
}
