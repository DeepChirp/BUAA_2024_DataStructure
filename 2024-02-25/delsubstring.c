#include <stdio.h>
#include <string.h>

#define MAX 200

int main()
{
    char str[MAX], substr[MAX];
    fgets(str, MAX, stdin);
    fgets(substr, MAX, stdin);
    // 去除换行符
    str[strlen(str) - 1] = '\0';
    substr[strlen(substr) - 1] = '\0';

    while (strstr(str, substr) != NULL)
    {
        char *p = strstr(str, substr);
        for (int i = 0; i < (int)strlen(substr); i++)
        {
            for (int j = p - str; j < (int)strlen(str) - 1; j++)
            {
                str[j] = str[j + 1];
            }
        }
        str[strlen(str) - strlen(substr)] = '\0';
    }

    printf("%s\n", str);

    return 0;
}
