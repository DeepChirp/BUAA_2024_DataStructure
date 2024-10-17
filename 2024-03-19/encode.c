#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_KEY_LENGTH 32

// 去除密钥中的重复字符
void removeDuplicates(char *key)
{
    int chars[128] = {0};
    int index = 0;
    for (int i = 0; key[i] != '\0'; i++)
    {
        if (!chars[(int)key[i]])
        {
            chars[(int)key[i]] = 1;
            key[index++] = key[i];
        }
    }
    key[index] = '\0';
}

// 移除字符
void removeChar(char *key, char c)
{
    int index = 0;
    for (int i = 0; key[i] != '\0'; i++)
    {
        if (key[i] != c)
        {
            key[index++] = key[i];
        }
    }
    key[index] = '\0';
}

// 生成环状字符表
char *generateCipherTable(char *key)
{
    char *table = (char *)malloc(256 * sizeof(char));
    strcpy(table, key);
    for (char c = 32; c <= 126; c++)
    {
        if (strchr(key, c) == NULL)
        {
            strncat(table, &c, 1);
        }
    }
    removeDuplicates(table);
    return table;
}

// 生成字符转换字符表
char *generateKey(char *key)
{
    char *table = (char *)malloc(256 * sizeof(char));
    int i = 0;
    int length = strlen(key);
    char firstChar = key[0];
    while (length > 1)
    {
        char c = key[i];
        removeChar(key, c);
        length = strlen(key);
        i = (i + c - 1) % length;
        table[c - 32] = key[i];
    }
    table[key[0] - 32] = firstChar;
    return table;
}

int main()
{
    char key[MAX_KEY_LENGTH + 1];
    char cipherTable[256];

    // 读取密钥
    fgets(key, sizeof(key), stdin);
    key[strcspn(key, "\n")] = '\0'; // 移除换行符

    // 生成环状字符表
    strcpy(cipherTable, generateCipherTable(key));

    // 生成字符转换字符表
    char *keyTable = generateKey(cipherTable);

    // 打开待加密文件和密文文件
    FILE *inputFile = fopen("in.txt", "r");
    FILE *outputFile = fopen("in_crpyt.txt", "w");

    if (inputFile == NULL || outputFile == NULL)
    {
        printf("Can't open the file.\n");
        return 1;
    }

    // 逐字符读取并加密文件内容
    int plainChar;
    while ((plainChar = fgetc(inputFile)) != EOF)
    {
        if (plainChar < 32 || plainChar > 126) // 非可打印字符
        {
            fputc(plainChar, outputFile);
        }
        else
        {
            fputc(keyTable[plainChar - 32], outputFile);
        }
    }

    // 关闭文件
    fclose(inputFile);
    fclose(outputFile);

    return 0;
}
