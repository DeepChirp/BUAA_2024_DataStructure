#include <stdio.h>
#include <string.h>

void generateKeyString(const char *key, char *keyString)
{
    int i, j;
    int keyLength = strlen(key);
    int alphabet[26] = {0}; // 记录密钥中出现的字母，避免重复

    // 将密钥中的字母追加到keyString的前面
    for (i = 0, j = 0; i < keyLength; i++)
    {
        if (!alphabet[key[i] - 'a'])
        {
            keyString[j++] = key[i];
            alphabet[key[i] - 'a'] = 1;
        }
    }

    // 将字母表中的其它字母以反序追加到keyString的后面
    for (i = 25, j = strlen(keyString); i >= 0; i--)
    {
        if (!alphabet[i])
        {
            keyString[j++] = i + 'a';
        }
    }
    keyString[j] = '\0';
}

int main()
{
    char key[55], keyString[27];

    scanf("%s", key);
    generateKeyString(key, keyString);

    // 读取文件encrypt.txt中内容
    FILE *fileIn = fopen("encrypt.txt", "r");
    FILE *fileOut = fopen("output.txt", "w");

    char ch;
    while ((ch = fgetc(fileIn)) != EOF)
    {
        if (ch >= 'a' && ch <= 'z')
        {
            fprintf(fileOut, "%c", keyString[ch - 'a']);
        }
        else
        {
            fprintf(fileOut, "%c", ch);
        }
    }

    fclose(fileIn);
    fclose(fileOut);

    return 0;
}
