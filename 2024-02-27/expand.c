#include <stdio.h>

int main()
{
    char s[100];
    scanf("%s", s);
    for (int i = 0; s[i] != '\0'; i++)
    {
        if (s[i] == '-' && i > 0 && s[i + 1] != '\0')
        {
            char prev = s[i - 1];
            char next = s[i + 1];
            if ((prev >= 'a' && prev <= 'z' && next >= 'a' && next <= 'z' && next > prev) ||
                (prev >= 'A' && prev <= 'Z' && next >= 'A' && next <= 'Z' && next > prev) ||
                (prev >= '0' && prev <= '9' && next >= '0' && next <= '9' && next > prev))
            {
                for (char c = prev + 1; c < next; c++)
                {
                    putchar(c);
                }
                continue;
            }
        }
        putchar(s[i]);
    }
    return 0;
}