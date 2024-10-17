#include <stdio.h>
#include <string.h>
#include <stdbool.h>
typedef struct WordList
{
    char *word;
    int count;
    struct WordList *next;
} list;
list *createNode(char *word)
{
    list *tmp = (char *)malloc(sizeof(list));
    tmp->word = (char *)malloc((strlen(word) + 1) * sizeof(char));
    strcpy(tmp->word, word);
    tmp->count = 1;
    return tmp;
}
int main()
{
    FILE *fp = fopen("article.txt())", "r");
    if (!fp)
    {
        printf("can't open file!\n");
        return 0;
    }
    rewind(fp);
    char line[1000];
    list *head = NULL;
    while (fgets(line, 1000, fp) != NULL)
    {
        int i = 0;
        char *words;
        int l = strlen(line);
        while (i < l && line[i] != '\n')
        { /*zhushi**/
            while (i < l && !isalpha(line[i]) && line[i] != '\n')
            {
                i++;
            }
            if (i >= l || line[i] == '\n')
                break; // /*/
            words = line + i;
            while (i < l && isalpha(line[i]))
            {
                if (isupper(line[i]))
                { // 如果是大写字母
                    line[i] = tolower(line[i]);
                }
                i++;
            }
            line[i] = '\0';
            // printf("%s\n",words);
            if (head == NULL)
            {
                head = (list *)malloc(sizeof(list) * 1);
                head->word = (char *)malloc((strlen(words) + 1) * sizeof(char));
                strcpy(head->word, words);
                head->count = 1;
                head->next = NULL;
            }
            else
            {
                list *pre = NULL;
                list *current = head;
                if (strcmp(words, current->word) < 0)
                { // 应该插入到头结点
                    list *tmp = createNode(words);
                    tmp->next = current;
                    head = tmp;
                }
                else
                {
                    while (current != NULL && (strcmp(words, current->word) > 0))
                    {
                        pre = current;
                        current = current->next;
                    }
                    if (current == NULL || strcmp(words, current->word) < 0)
                    { // 插入到末尾或者是current之前

                        list *tmp = createNode(words);
                        pre->next = tmp;
                        tmp->next = current;
                    }
                    else
                    { // 找到了对应的单词 对应的count值增加
                        (current->count)++;
                    }
                }
            }
            i++;
        }
    }
    list *tmp = head;
    while (tmp != NULL)
    {
        printf("%s %d\n", tmp->word, tmp->count);
        tmp = tmp->next;
    }
}
