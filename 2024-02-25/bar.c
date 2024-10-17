#include <stdio.h>

int main()
{
    int counts[26] = {0};
    int max_count = 0;
    char c;

    while ((c = getchar()) != EOF)
    {
        if (c >= 'a' && c <= 'z')
        {
            counts[c - 'a']++;
            if (counts[c - 'a'] > max_count)
            {
                max_count = counts[c - 'a'];
            }
        }
    }

    for (int i = max_count; i > 0; i--)
    {
        for (int j = 0; j < 26; j++)
        {
            if (counts[j] >= i)
            {
                putchar('*');
            }
            else
            {
                putchar(' ');
            }
        }
        putchar('\n');
    }

    for (char i = 'a'; i <= 'z'; i++)
    {
        putchar(i);
    }

    return 0;
}