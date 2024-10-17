#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE 100

void trim(char *str)
{
    char *p = str;
    char *q = str;
    int space = 0;

    while (*p)
    {
        if (isspace(*p))
        {
            if (space == 0 && q > str)
            {
                *q++ = ' ';
                space = 1;
            }
        }
        else
        {
            *q++ = *p;
            space = 0;
        }
        p++;
    }

    if (q > str && isspace(*(q - 1)))
        q--;
    *q = '\0';
}

int main()
{
    int pos;
    scanf("%d", &pos);
    getchar();

    char line[MAX_LINE];
    while (fgets(line, MAX_LINE, stdin))
    {
        char *colon = strchr(line, ':');
        if (colon)
        {
            *colon = '\0';
            char *left = line;
            char *right = colon + 1;

            trim(left);
            trim(right);

            printf("%*s : %s\n", pos - 2, left, right);
        }
    }

    return 0;
}