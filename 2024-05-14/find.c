#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_WORD_COUNT 3500
#define MAX_WORD_LENGTH 20

#define NHASH 3001
#define MULT 37

char words[MAX_WORD_COUNT][MAX_WORD_LENGTH];
int alpha_index[27] = {0};

typedef struct Node
{
    char str[MAX_WORD_LENGTH];
    struct Node *next;
} Node;

Node *hash_table[NHASH];

unsigned int hash(char *str);
void add_to_hashtable(char *str);
bool binary_search(char word[], int left, int right, int *count);

/* compute hash value for string */
unsigned int hash(char *str)
{
    unsigned int h = 0;
    char *p;
    for (p = str; *p != '\0'; p++)
    {
        h = MULT * h + (unsigned int)*p;
    }
    return h % NHASH;
}

void add_to_hashtable(char *str)
{
    unsigned int h = hash(str);
    Node *new_node = (Node *)malloc(sizeof(Node));
    strcpy(new_node->str, str);
    new_node->next = hash_table[h];
    hash_table[h] = new_node;
}

bool binary_search(char word[], int left, int right, int *count)
{
    while (left <= right)
    {
        (*count)++;
        int mid = (left + right) / 2;
        int cmp = strcmp(words[mid], word);
        if (cmp == 0)
        {
            return true;
        }
        else if (cmp < 0)
        {
            left = mid + 1;
        }
        else
        {
            right = mid - 1;
        }
    }
    return false;
}

int main()
{
    // initialize the hash table
    for (int i = 0; i < NHASH; i++)
    {
        hash_table[i] = NULL;
    }

    // initialize the words
    int word_sum = 0;
    FILE *fp = fopen("dictionary3000.txt", "r");
    char line[MAX_WORD_LENGTH + 2];
    char alpha = 'a';
    while (fgets(line, sizeof(line), fp) != NULL)
    {
        line[strcspn(line, "\n")] = 0;
        line[strcspn(line, "\r")] = 0;

        // avoid empty line
        if (line[0] != 0)
        {
            strcpy(words[word_sum++], line);

            // establish the index of the first word of each alpha
            if (line[0] != alpha)
            {
                int new_alpha = line[0];
                for (int i = alpha - 'a' + 1; i <= new_alpha - 'a'; i++)
                {
                    alpha_index[i] = word_sum - 1;
                }
                alpha = (char)new_alpha;
            }

            // establish hash table
            add_to_hashtable(line);
        }
    }
    alpha_index[26] = word_sum - 1; // avoid overflow
    fclose(fp);

    // read the input
    int method;
    char word[MAX_WORD_LENGTH];
    Node *node;
    while (scanf("%s %d", word, &method) != EOF)
    {
        int count = 0; // count the comparaison times
        bool found = false;
        switch (method)
        {
        case 1: // search by line order
            for (int i = 0; i < word_sum; i++)
            {
                count++;
                int cmp = strcmp(words[i], word);
                if (cmp == 0)
                {
                    found = true;
                    break;
                }
                else if (cmp > 0)
                {
                    break;
                }
            }
            break;

        case 2: // search by binary search
            found = binary_search(word, 0, word_sum - 1, &count);
            break;

        case 3: // search by index
            found = binary_search(word, alpha_index[word[0] - 'a'], alpha_index[word[0] - 'a' + 1] - 1, &count);
            break;

        case 4: // search by hash
            node = hash_table[hash(word)];
            while (node != NULL)
            {
                count++;
                if (strcmp(node->str, word) == 0)
                {
                    found = true;
                    break;
                }
                node = node->next;
            }
            break;

        default:
            break;
        }
        printf("%d %d\n", found, count);
    }

    return 0;
}
