#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define MAX_WORD_LENGTH 100

// 定义单词节点
typedef struct WordNode
{
    char word[MAX_WORD_LENGTH];
    int count;
    struct WordNode *next;
} WordNode;

// 插入单词节点到链表中
void insertWord(WordNode **head, char *word)
{
    WordNode *current = *head;
    WordNode *prev = NULL;

    // 寻找插入位置
    while (current != NULL && strcmp(word, current->word) > 0)
    {
        prev = current;
        current = current->next;
    }

    // 如果单词已经存在，则增加计数
    if (current != NULL && strcmp(word, current->word) == 0)
    {
        current->count++;
    }
    else
    {
        // 否则创建新的节点并插入链表
        WordNode *newNode = (WordNode *)malloc(sizeof(WordNode));
        strcpy(newNode->word, word);
        newNode->count = 1;
        newNode->next = current;

        if (prev == NULL)
        {
            *head = newNode;
        }
        else
        {
            prev->next = newNode;
        }
    }
}

int main()
{
    // 打开文件
    FILE *file = fopen("article.txt", "r");
    if (file == NULL)
    {
        perror("Can't open the file\n");
        return 1;
    }

    // 创建链表头
    WordNode *head = NULL;
    char word[MAX_WORD_LENGTH];

    // 从文件中读取单词并统计词频
    char ch;
    while ((ch = fgetc(file)) != EOF)
    {
        if (!isalpha(ch))
        {
            continue;
        }
        else
        {
            int i = 0;
            if (isupper(ch))
            {
                ch = tolower(ch);
            }
            word[i++] = ch;
            while ((ch = fgetc(file)) != EOF && isalpha(ch))
            {
                if (isupper(ch))
                {
                    ch = tolower(ch);
                }
                word[i++] = ch;
            }
            word[i] = '\0';
            insertWord(&head, word); // 插入单词到链表中
        }
    }

    // 关闭文件
    fclose(file);

    // 输出结果
    WordNode *current = head;
    while (current != NULL)
    {
        printf("%s %d\n", current->word, current->count);
        current = current->next;
    }

    return 0;
}
