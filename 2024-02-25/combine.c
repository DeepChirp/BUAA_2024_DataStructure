#include <stdio.h>
#include <string.h>

void str_bin(char str1[], char str2[])
{
    int length1 = strlen(str1);
    int length2 = strlen(str2);
    int i = 0, j = 0;
    while (i < length1 && j < length2)
    {
        if (str1[i] < str2[j])
        {
            putchar(str1[i]);
            i++;
        }
        else
        {
            putchar(str2[j]);
            j++;
        }
    }
    while (i < length1)
    {
        putchar(str1[i]);
        i++;
    }
    while (j < length2)
    {
        putchar(str2[j]);
        j++;
    }
}

int main()
{
    char str1[105], str2[105];
    scanf("%s%s", str1, str2);
    str_bin(str1, str2);

    return 0;
}
