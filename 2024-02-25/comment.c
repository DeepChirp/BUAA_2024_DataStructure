#include <stdio.h>

int main()
{
    FILE *fp = fopen("filein.c", "r");
    if (fp == NULL)
    {
        perror("File not found\n");
        return 1;
    }
    char c;
    int isComment = 0, sum = 0, comment = 0;
    while ((c = getc(fp)) != EOF)
    {
        if (!isComment)
        {
            if (c == '/')
            {
                c = getc(fp);
                if (c == '*')
                {
                    isComment = 1;
                }
                sum += 2;
            }
            else
            {
                sum++;
            }
        }
        else
        {
            if (c == '*')
            {
                c = getc(fp);
                if (c == '/')
                {
                    isComment = 0;
                }
                else
                {
                    comment += 2;
                }
                sum += 2;
            }
            else
            {
                comment++;
                sum++;
            }
        }
    }
    printf("%d%%\n", comment * 100 / sum);

    return 0;
}
