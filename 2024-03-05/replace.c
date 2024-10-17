#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

// 将字符串转换为小写
char *toLower(const char *str)
{
    int len = strlen(str);
    char *lowerStr = (char *)malloc(len + 1);
    if (lowerStr != NULL)
    {
        for (int i = 0; i < len; i++)
        {
            lowerStr[i] = tolower(str[i]);
        }
        lowerStr[len] = '\0';
    }
    return lowerStr;
}

// 替换字符串中的某个单词
char *replaceWord(const char *s, const char *oldW, const char *newW)
{
    char *result;
    int i, cnt = 0;
    int newWlen = strlen(newW);
    int oldWlen = strlen(oldW);

    char *lowerS = toLower(s);
    char *lowerOldW = toLower(oldW);

    // 计算旧单词在字符串中出现的次数
    for (i = 0; lowerS[i] != '\0'; i++)
    {
        if (strstr(&lowerS[i], lowerOldW) == &lowerS[i])
        {
            cnt++;
            i += oldWlen - 1;
        }
    }

    // 分配足够的内存来存储替换后的字符串
    result = (char *)malloc(i + cnt * (newWlen - oldWlen) + 1);

    i = 0;
    // 替换字符串中的旧单词
    while (*s)
    {
        if (strstr(lowerS, lowerOldW) == lowerS)
        {
            strcpy(&result[i], newW);
            i += newWlen;
            s += oldWlen;
            lowerS += oldWlen;
        }
        else
        {
            result[i++] = *s++;
            lowerS++;
        }
    }

    result[i] = '\0';
    // free(lowerS);
    free(lowerOldW);
    return result;
}

int main()
{
    // 打开输入和输出文件
    FILE *fileIn = fopen("filein.txt", "r");
    FILE *fileOut = fopen("fileout.txt", "w");
    char oldW[100], newW[100], buffer[4096];

    // 从控制台读取旧单词和新单词
    scanf("%s", oldW);
    scanf("%s", newW);

    // 读取输入文件的每一行，并替换旧单词
    while ((fgets(buffer, 4096, fileIn)) != NULL)
    {
        char *newBuffer = replaceWord(buffer, oldW, newW);
        fputs(newBuffer, fileOut);
        free(newBuffer);
    }

    // 关闭文件
    fclose(fileIn);
    fclose(fileOut);

    return 0;
}