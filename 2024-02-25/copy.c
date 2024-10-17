#include <stdio.h>

int main()
{
    FILE *in = fopen("fcopy.in", "r");
    FILE *out = fopen("fcopy.out", "w");

    char c;
    int isBlank = 0;
    while ((c = fgetc(in)) != EOF)
    {
        if (c == ' ' || c == '\t')
        {
            if (isBlank == 0)
            {
                fputc(' ', out);
                isBlank = 1;
            }
            else
            {
                continue;
            }
        }
        else
        {
            fputc(c, out);
            isBlank = 0;
        }
    }

    return 0;
}
