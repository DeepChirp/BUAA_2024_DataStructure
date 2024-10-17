#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct Phone
{
    char name[25];
    char number[20];
};

int cmp(const void *a, const void *b)
{
    return strcmp(((struct Phone *)a)->name, ((struct Phone *)b)->name);
}

int main()
{
    int n;
    scanf("%d", &n);

    struct Phone phones[100];
    int count[100] = {0};

    for (int i = 0; i < n; i++)
    {
        scanf("%s %s", phones[i].name, phones[i].number);
    }

    // 删去重复记录
    for (int i = 0; i < n; i++)
    {
        for (int j = i + 1; j < n; j++)
        {
            // 如果重名且重号码，则删除后一个记录
            if (strcmp(phones[i].name, phones[j].name) == 0 && strcmp(phones[i].number, phones[j].number) == 0)
            {
                for (int k = j; k < n - 1; k++)
                {
                    strcpy(phones[k].name, phones[k + 1].name);
                    strcpy(phones[k].number, phones[k + 1].number);
                }
                n--;
                j--;
            }
        }
    }

    // 对重名记录添加后缀
    for (int i = 0; i < n; i++)
    {
        for (int j = i + 1; j < n; j++)
        {
            //  如果重名但不重号码，则添加后缀
            if (strcmp(phones[i].name, phones[j].name) == 0)
            {
                count[j]++;
                char suffix[4];
                sprintf(suffix, "_%d", count[j]);

                // 检查是否已经有后缀
                char *suffix_position = strrchr(phones[j].name, '_');
                if (suffix_position != NULL)
                {
                    *suffix_position = '\0';
                }

                strcat(phones[j].name, suffix);
            }
        }
    }

    // 整理排序
    qsort(phones, n, sizeof(struct Phone), cmp);

    // 输出
    for (int i = 0; i < n; i++)
    {
        printf("%s %s\n", phones[i].name, phones[i].number);
    }

    return 0;
}