#pragma GCC optimize(1)
#pragma GCC optimize(2)
#pragma GCC optimize(3)
#pragma GCC optimize("Ofast")
#pragma GCC optimize("inline")
#pragma GCC optimize("-fgcse")
#pragma GCC optimize("-fgcse-lm")
#pragma GCC optimize("-fipa-sra")
#pragma GCC optimize("-ftree-pre")
#pragma GCC optimize("-ftree-vrp")
#pragma GCC optimize("-fpeephole2")
#pragma GCC optimize("-ffast-math")
#pragma GCC optimize("-fsched-spec")
#pragma GCC optimize("unroll-loops")
#pragma GCC optimize("-falign-jumps")
#pragma GCC optimize("-falign-loops")
#pragma GCC optimize("-falign-labels")
#pragma GCC optimize("-fdevirtualize")
#pragma GCC optimize("-fcaller-saves")
#pragma GCC optimize("-fcrossjumping")
#pragma GCC optimize("-fthread-jumps")
#pragma GCC optimize("-funroll-loops")
#pragma GCC optimize("-freorder-blocks")
#pragma GCC optimize("-fschedule-insns")
#pragma GCC optimize("inline-functions")
#pragma GCC optimize("-ftree-tail-merge")
#pragma GCC optimize("-fschedule-insns2")
#pragma GCC optimize("-fstrict-aliasing")
#pragma GCC optimize("-falign-functions")
#pragma GCC optimize("-fcse-follow-jumps")
#pragma GCC optimize("-fsched-interblock")
#pragma GCC optimize("-fpartial-inlining")
#pragma GCC optimize("no-stack-protector")
#pragma GCC optimize("-freorder-functions")
#pragma GCC optimize("-findirect-inlining")
#pragma GCC optimize("-fhoist-adjacent-loads")
#pragma GCC optimize("-frerun-cse-after-loop")
#pragma GCC optimize("inline-small-functions")
#pragma GCC optimize("-finline-small-functions")
#pragma GCC optimize("-ftree-switch-conversion")
#pragma GCC optimize("-foptimize-sibling-calls")
#pragma GCC optimize("-fexpensive-optimizations")
#pragma GCC optimize("inline-functions-called-once")
#pragma GCC optimize("-fdelete-null-pointer-checks")

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <math.h>

#define Max(a, b) ((a) > (b) ? (a) : (b))
#define Min(a, b) ((a) < (b) ? (a) : (b))

#define MAX_FUNC_COUNT 32
#define MAX_FUNC_NAME_LENGTH 128
#define MAX_PROGRAM_COUNT 256
#define MAX_PROGRAM_ID_LENGTH 32
#define MAX_WORD_COUNT 256
#define MAX_WORD_LENGTH 128
#define MAX_LINE_LENGTH 2048
#define MAX_LINE_COUNT 512
#define MAX_IDENTIFIER_LENGTH 128

#define CODE_FILE "codes.txt"
#define KEEP_WORD_FILE "keepwords.txt"
#define THRESHOLD 0.95

int program_sum = 0;
char function_names[MAX_FUNC_COUNT][MAX_FUNC_NAME_LENGTH];

int buf[MAX_LINE_LENGTH + 1];

struct Program
{
    char id[MAX_PROGRAM_ID_LENGTH];
    char code[MAX_LINE_COUNT][MAX_LINE_LENGTH];
    int line_count;
    char function_key_info[MAX_FUNC_COUNT][MAX_LINE_LENGTH];
    int function_count;
    int function_count_in_main;
    char key_info[MAX_LINE_LENGTH];
    unsigned int key_info_len;
    int key_info_character_count[95]; // ASCII characters from 32 to 126
};

struct Program programs[MAX_PROGRAM_COUNT];

// Use trie to store the keep words
typedef struct TrieNode
{
    struct TrieNode *children[27]; // Assuming only lowercase letters and underscore
    bool isEndOfWord;              // true if the node represents end of a word
} TrieNode;

TrieNode *root = NULL;

TrieNode *GetNode(void);
void Insert(char *word);
void LoadKeepWord(const char *file_name);
bool IsKeepWord(char *word);
void GetkeyInformationFlow(const char *file_name);
void CalculateSimilarityAndClassify();
bool IsSimilar(char *str1, char *str2, int len1, int len2, int max_distance);

int main()
{
    LoadKeepWord(KEEP_WORD_FILE);
    GetkeyInformationFlow(CODE_FILE);
    CalculateSimilarityAndClassify();

    return 0;
}
TrieNode *GetNode(void)
{
    TrieNode *node = (TrieNode *)malloc(sizeof(TrieNode));
    node->isEndOfWord = false;

    for (int i = 0; i < 27; i++) // Initialize all 27 children to NULL
    {
        node->children[i] = NULL;
    }

    return node;
}

void Insert(char *word)
{
    TrieNode *node = root;
    for (int i = 0; i < (int)strlen(word); i++)
    {
        int index = word[i] == '_' ? 26 : word[i] - 'a';
        if (!node->children[index])
        {
            node->children[index] = GetNode();
        }
        node = node->children[index];
    }
    node->isEndOfWord = true;
}

void LoadKeepWord(const char *file_name)
{
    root = GetNode(); // Use the global root node
    FILE *fp = fopen(file_name, "r");
    char word[MAX_WORD_LENGTH];
    // Read the keep words from the file line by line
    while (fscanf(fp, "%s", word) != EOF)
    {
        Insert(word);
    }
    fclose(fp);
}

bool IsKeepWord(char *word)
{
    TrieNode *node = root;
    for (int i = 0; i < (int)strlen(word); i++)
    {
        if (!islower(word[i]) && word[i] != '_') // If the character is not lowercase or underscore, return false
        {
            return false;
        }

        int index = word[i] == '_' ? 26 : word[i] - 'a';
        if (!node->children[index])
        {
            return false;
        }

        node = node->children[index];
    }

    return (node != NULL && node->isEndOfWord);
}

void GetkeyInformationFlow(const char *file_name)
{
    char line[MAX_LINE_LENGTH];
    FILE *fp;
    fp = fopen(file_name, "r");

    int is_new_program = true;
    while (1)
    {
        if (is_new_program)
        {
            // Skip all empty lines
            while (fgets(line, sizeof(line), fp) != NULL && (line[0] == '\n' || line[0] == '\r'))
                ;

            // Now, line should contain the program number
            // Copy the program number, excluding any newline or carriage return
            strcpy(programs[program_sum].id, line);
            strstr(programs[program_sum].id, "\r") ? *strstr(programs[program_sum].id, "\r") = '\0' : 0;
            strstr(programs[program_sum].id, "\n") ? *strstr(programs[program_sum].id, "\n") = '\0' : 0;

            is_new_program = false;
            program_sum++;
        }

        // Find all user-defined functions
        int brace_count = 0;
        char function_name[MAX_WORD_LENGTH];
        int name_index = 0;
        bool is_function_name = false;
        bool should_skip = false;
        int main_index = -1;

        while (fgets(line, sizeof(line), fp) != NULL)
        {
            if (line[0] == '\f')
            {
                is_new_program = true;
                break;
            }
            for (int i = 0; line[i] != '\0'; i++)
            {
                if (line[i] == '{')
                {
                    brace_count++;
                }
                else if (line[i] == '}')
                {
                    brace_count--;
                    if (brace_count == 0)
                    {
                        should_skip = false;
                    }
                }
                else if (!should_skip && line[i] != ' ' && line[i] != '\t' && line[i] != '\n' && line[i] != '\r')
                {
                    if (line[i] == '(' || line[i] == '{')
                    {
                        if (is_function_name)
                        {
                            function_name[name_index] = '\0';
                            strcpy(function_names[programs[program_sum - 1].function_count++], function_name);
                            is_function_name = false;
                            name_index = 0;
                            should_skip = true;
                            if (strcmp(function_name, "main") == 0)
                            {
                                main_index = programs[program_sum - 1].function_count - 1;
                            }
                        }
                        if (line[i] == '{')
                        {
                            brace_count++;
                        }
                    }
                    else
                    {
                        is_function_name = true;
                        function_name[name_index++] = line[i];
                    }
                }
            }
            // Put the content into the code buffer
            strcpy(programs[program_sum - 1].code[programs[program_sum - 1].line_count++], line);
        }

        // Read the file character by character
        char c;
        char identifier[MAX_IDENTIFIER_LENGTH];
        int function_in_main[MAX_FUNC_COUNT];
        int identifier_length = 0;
        int function_key_info_index = 0;
        int function_index = -1;
        bool is_in_main = false;

        for (int code_index = 0; code_index < MAX_LINE_COUNT; code_index++)
        {
            int char_index = 0;
            while ((c = programs[program_sum - 1].code[code_index][char_index++]) != '\0')
            {
                // If the character is a brace, update the brace count
                if (c == '{')
                {
                    brace_count++;
                }
                else if (c == '}')
                {
                    brace_count--;
                }

                // If the character is a letter, it might be the start of an identifier
                if (isalpha(c) || (identifier_length > 0 && (isalnum(c) || c == '_')))
                {
                    // Add the character to the identifier
                    identifier[identifier_length++] = c;
                }
                else
                {
                    // If we have an identifier, process it
                    if (identifier_length > 0)
                    {
                        // Null-terminate the identifier
                        identifier[identifier_length] = '\0';

                        if (brace_count == 0)
                        {
                            // Check if the identifier is a function name
                            for (int i = 0; i < programs[program_sum - 1].function_count; i++)
                            {
                                if (strcmp(identifier, function_names[i]) == 0)
                                {
                                    if (i == main_index)
                                    {
                                        is_in_main = true;
                                    }
                                    else
                                    {
                                        is_in_main = false;
                                    }
                                    function_index++;
                                    function_key_info_index = 0;
                                    break;
                                }
                            }
                        }
                        else
                        {
                            if (IsKeepWord(identifier))
                            {
                                strcat(programs[program_sum - 1].function_key_info[function_index], identifier);
                                function_key_info_index += (int)strlen(identifier);
                            }
                            else
                            {
                                // Check if the identifier is a function name
                                is_function_name = false;
                                int i;
                                for (i = 0; i < programs[program_sum - 1].function_count; i++)
                                {
                                    if (strcmp(identifier, function_names[i]) == 0)
                                    {
                                        is_function_name = true;
                                        break;
                                    }
                                }
                                if (is_function_name)
                                {
                                    if (is_in_main)
                                    {
                                        // Add the function to the list of functions in main
                                        int j;
                                        for (j = 0; j < programs[program_sum - 1].function_count_in_main; j++)
                                        {
                                            if (function_in_main[j] == i)
                                            {
                                                break;
                                            }
                                        }
                                        if (j == programs[program_sum - 1].function_count_in_main)
                                        {
                                            function_in_main[programs[program_sum - 1].function_count_in_main] = i;
                                            programs[program_sum - 1].function_count_in_main++;
                                        }
                                    }
                                    strcat(programs[program_sum - 1].function_key_info[function_index], "FUNC");
                                    function_key_info_index += 4;
                                }
                            }
                        }

                        // Reset the identifier length
                        identifier_length = 0;
                    }
                    if (brace_count != 0 || c == '}')
                    {
                        // Remove all whitespace characters
                        if (c != '\n' && c != '\r' && c != '\t' && c != ' ')
                        {
                            programs[program_sum - 1].function_key_info[function_index][function_key_info_index++] = c;
                        }
                    }
                }
            }
        }

        // Add the key information of the main function to the program's key information
        strcat(programs[program_sum - 1].key_info, programs[program_sum - 1].function_key_info[main_index]);

        // Add the key information of the functions called in the main function to the program's key information
        for (int i = 0; i < programs[program_sum - 1].function_count_in_main; i++)
        {
            strcat(programs[program_sum - 1].key_info, programs[program_sum - 1].function_key_info[function_in_main[i]]);
        }

        programs[program_sum - 1].key_info_len = (unsigned int)strlen(programs[program_sum - 1].key_info);

        for (int i = 0; i < (int)programs[program_sum - 1].key_info_len; i++)
        {
            programs[program_sum - 1].key_info_character_count[(int)programs[program_sum - 1].key_info[i] - ' ']++;
        }

        // If there is no more content, exit the loop
        if (feof(fp))
        {
            break;
        }
    }
    fclose(fp);
}

int program_classes[MAX_PROGRAM_COUNT][MAX_PROGRAM_COUNT] = {{0}}; // 2D array to keep track of which classes each program belongs to
int class_sizes[MAX_PROGRAM_COUNT] = {0};                          // Array to keep track of the size of each class
bool is_visited[MAX_PROGRAM_COUNT] = {false};                      // Array to keep track of which programs have been visited

void CalculateSimilarityAndClassify()
{
    double sim;

    for (int i = 0; i < program_sum; i++)
    {
        if (is_visited[i])
        {
            continue;
        }
        program_classes[i][i] = 1; // The program is in the class with itself
        class_sizes[i] = 1;
        is_visited[i] = true;

        for (int j = i + 1; j < program_sum; j++)
        {
            /*
            If P1 is the substring of P2, the similarity between P1 and P2 is sim = program_A_key_info_len / program_B_key_info_len. So the similarity is less than program_A_key_info_len / program_B_key_info_len.
            The same applies to P2 is the substring of P1.
            sim <= min(program_A_key_info_len, program_B_key_info_len) / max(program_A_key_info_len, program_B_key_info_len)
            */
            if (programs[i].key_info_len < programs[j].key_info_len)
            {
                sim = (double)programs[i].key_info_len / programs[j].key_info_len;
            }
            else
            {
                sim = (double)programs[j].key_info_len / programs[i].key_info_len;
            }

            if (sim <= THRESHOLD)
            {
                continue;
            }

            /*
            One character difference can be corrected by adding or removing a character, and one editing operation can correct two character differences at most.
            edit_distance >= (character_difference + 1) / 2.
            */
            int character_difference = 0;
            for (int k = 0; k < 95; k++)
            {
                character_difference += abs(programs[i].key_info_character_count[k] - programs[j].key_info_character_count[k]);
            }

            if (1 - ((character_difference + 1) / 2) / (double)Max(programs[i].key_info_len, programs[j].key_info_len) <= THRESHOLD)
            {
                continue;
            }

            int min_edit_distance = (int)((1 - THRESHOLD) * Max(programs[i].key_info_len, programs[j].key_info_len));

            // Classify programs
            if (IsSimilar(programs[i].key_info, programs[j].key_info, (int)programs[i].key_info_len, (int)programs[j].key_info_len, min_edit_distance))
            {
                program_classes[i][j] = 1;
                class_sizes[i]++;
                is_visited[j] = true;
            }
        }
    }

    // Output classes
    for (int i = 0; i < program_sum; i++)
    {
        // Check if the class is a subset of any previous class
        bool is_subset = false;
        for (int j = 0; j < i; j++)
        {
            if (class_sizes[j] < class_sizes[i]) // If the previous class is smaller, skip it
            {
                continue;
            }

            is_subset = true;
            for (int k = 0; k < program_sum; k++)
            {
                if (program_classes[i][k] && !program_classes[j][k])
                {
                    is_subset = false;
                    break;
                }
            }

            if (is_subset)
            {
                break;
            }
        }

        if (!is_subset && class_sizes[i] > 1)
        {
            for (int j = 0; j < program_sum; j++)
            {
                if (program_classes[i][j])
                {
                    printf("%s ", programs[j].id);
                }
            }
            putchar('\n');
        }
    }
}

/**
 * @brief Checks if two strings are similar within a given maximum distance.
 *
 * This function uses the Wagner-Fischer algorithm to calculate the edit distance between two strings.
 *
 * @param s1 The first string.
 * @param s2 The second string.
 * @param len1 Length of the first string.
 * @param len2 Length of the second string.
 * @param max_distance Maximum allowed distance between the strings.
 *
 * @return True if the strings are similar within the maximum distance, false otherwise.
 *
 * @details
 * The function calculates the edit distance between the two strings. If the distance is less than or equal to the maximum distance, the function returns true. Otherwise, it returns false.
 *
 * @see
 * - [Wagner–Fischer algorithm on Wikipedia](https://en.wikipedia.org/wiki/Wagner%E2%80%93Fischer_algorithm)
 * - [Wagner–Fischer algorithm on SIAM](https://epubs.siam.org/doi/epdf/10.1137/S0097539794264810)
 * - [A method to optimize the Wagner-Fischer algorithm](https://ceptord.net/wagner-fischer/index.html)
 * - [A way to return result in advance on Stack Overflow](https://stackoverflow.com/questions/48901351/levenstein-distance-limit)
 *
 * @note
 * Some codes are from https://ceptord.net/wagner-fischer/index.html, which are under MIT License. And the documentation itself is under public domain.
 */
bool IsSimilar(char *s1, char *s2, int len1, int len2, int max_distance)
{
    int i, j, diagonal, temp;

    if (len1 > len2)
    {
        return IsSimilar(s2, s1, len2, len1, max_distance);
    }

    int max_index = max_distance;
    int min_index = -max_distance;

    int result_diag = len2 - len1;

    if (result_diag > max_distance)
    {
        return false;
    }

    int current_max = Min(max_index, len2);
    int current_min = Max(min_index, 0);
    int result_index = Min(result_diag, len2);

    for (j = current_min; j <= current_max; j++)
    {
        buf[j] = j;
    }
    buf[current_max + 1] = max_distance + 1;

    for (i = 1; i <= len1; i++)
    {
        max_index++;
        min_index++;
        result_diag++;

        current_max = Min(max_index, len2);
        current_min = Max(min_index, 1);
        result_index = Min(result_diag, len2);

        if (current_min == 1)
        {
            diagonal = i - 1;
            buf[0] = i;
        }
        else
        {
            diagonal = buf[j - 1];
            buf[current_min - 1] = max_distance + 1;
        }

        // If the distance is greater than the maximum distance, return false.
        for (j = current_min; j <= current_max; j++)
        {
            temp = buf[j];
            if (s1[i - 1] == s2[j - 1])
            {
                buf[j] = diagonal;
            }
            else
            {
                buf[j] = Min(Min(diagonal, buf[j - 1]), buf[j]) + 1; // Min3(diagonal, buf[j - 1], buf[j]) + 1
            }
            diagonal = temp;
        }

        if (buf[result_index] > max_distance)
        {
            return false;
        }

        buf[current_max + 1] = max_distance + 1;
    }

    return buf[len2] <= max_distance;
}