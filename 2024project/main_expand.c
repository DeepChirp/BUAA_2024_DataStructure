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

#define max2(a, b) ((a) > (b) ? (a) : (b))

#define MAX_FILE_NAME_LENGTH 512
#define MAX_FUNC_COUNT 10000
#define MAX_FUNC_NAME_LENGTH 512
#define MAX_PROGRAM_COUNT 10000
#define MAX_PROGRAM_ID_LENGTH 512
#define MAX_WORD_COUNT 10000
#define MAX_WORD_LENGTH 512
#define MAX_LINE_LENGTH 1000000
#define MAX_IDENTIFIER_LENGTH 512

#define WORD_FILE "keepwords.txt"
#define CODE_FILE "codes.txt"
#define THRESHOLD 0.95

char *program_id[MAX_PROGRAM_COUNT];
int program_sum = 0;
char functionNames[MAX_FUNC_COUNT][MAX_FUNC_NAME_LENGTH];
int functionCount = 0;
char keepWords[MAX_WORD_COUNT][MAX_WORD_LENGTH];
int keepWordCount = 0;

void LoadKeepWord(const char *filename);
void SpiltFile(const char *filename);
bool IsKeepWord(char *word);
void GetKeyInformationFlow(char *file_id);
void CalculateSimilarityAndClassify();
int Min3(int a, int b, int c);
void Error2(const char *s) __attribute__((noreturn));
int InitDP();
int EditdistDP(char *str1, char *str2);

int main()
{
    LoadKeepWord(WORD_FILE);
    SpiltFile(CODE_FILE);
    for (int i = 0; i < program_sum; i++)
    {
        GetKeyInformationFlow(program_id[i]);
    }
    CalculateSimilarityAndClassify();

    return 0;
}

void LoadKeepWord(const char *filename)
{
    FILE *fp = fopen(filename, "r");
    if (fp == NULL)
    {
        printf("Failed to open file: %s\n", filename);
        return;
    }

    // Read the keep words from the file line by line
    char line[MAX_WORD_LENGTH];
    while (fscanf(fp, "%s", line) != EOF)
    {
        strcpy(keepWords[keepWordCount++], line);
    }
    fclose(fp);
}

void SpiltFile(const char *filename)
{
    char line[MAX_LINE_LENGTH];
    FILE *fp;
    fp = fopen(filename, "r");
    if (fp == NULL)
    {
        printf("Failed to open file: %s\n", filename);
        return;
    }

    int is_new_program = 1;

    while (1)
    {
        if (is_new_program)
        {
            // Skip all empty lines
            while (fgets(line, sizeof(line), fp) != NULL && (line[0] == '\n' || line[0] == '\r'))
                ;

            // If there is no more content, exit the loop
            if (feof(fp))
            {
                break;
            }

            // Now, line should contain the program number
            program_id[program_sum] = malloc(MAX_WORD_LENGTH * sizeof(char));

            // Copy the program number, excluding any newline or carriage return
            strncpy(program_id[program_sum], line, strcspn(line, "\r\n"));

            // Ensure the string is null-terminated
            program_id[program_sum][strcspn(line, "\r\n")] = '\0';

            is_new_program = 0;
            program_sum++;
        }
        char temp_filename[MAX_FILE_NAME_LENGTH];
        sprintf(temp_filename, "temp_program%s.c", program_id[program_sum - 1]);
        FILE *temp_fp = fopen(temp_filename, "w");
        while (fgets(line, sizeof(line), fp) != NULL)
        {
            if (line[0] == '\f')
            {
                is_new_program = 1;
                break;
            }
            fputs(line, temp_fp);
        }
        fclose(temp_fp);

        // If there is no more content, exit the loop
        if (feof(fp))
        {
            break;
        }
    }
    fclose(fp);
}

bool IsKeepWord(char *word)
{
    for (int i = 0; i < keepWordCount; i++)
    {
        if (strcmp(word, keepWords[i]) == 0)
        {
            return true;
        }
    }
    return false;
}

void GetKeyInformationFlow(char *file_id)
{
    char original_file_name[MAX_FILE_NAME_LENGTH];
    char function_file_name[MAX_FILE_NAME_LENGTH];
    char program_file_name[MAX_FILE_NAME_LENGTH];
    sprintf(original_file_name, "temp_program%s.c", file_id);
    sprintf(function_file_name, "temp_program%s_function.c", file_id);
    sprintf(program_file_name, "temp_program%s_program.c", file_id);

    FILE *file = fopen(original_file_name, "r");
    char line[MAX_LINE_LENGTH];
    int braceCount = 0;
    char functionName[256];
    int nameIndex = 0;
    bool collectingName = false;
    functionCount = 0;

    // find all user-defined functions
    while (fgets(line, sizeof(line), file))
    {
        for (int i = 0; i < (int)strlen(line); i++)
        {
            if (line[i] == '{')
            {
                braceCount++;
                if (collectingName)
                {
                    functionName[nameIndex] = '\0';
                    strcpy(functionNames[functionCount++], functionName);
                    collectingName = false;
                    nameIndex = 0;
                }
            }
            else if (line[i] == '}')
            {
                braceCount--;
            }
            else if (braceCount == 0 && line[i] != ' ' && line[i] != '\t' && line[i] != '\n' && line[i] != '\r')
            {
                if (line[i] == '(')
                {
                    if (collectingName)
                    {
                        functionName[nameIndex] = '\0';
                        strcpy(functionNames[functionCount++], functionName);
                        collectingName = false;
                        nameIndex = 0;
                    }
                }
                else
                {
                    if (!collectingName)
                    {
                        collectingName = true;
                    }
                    functionName[nameIndex++] = line[i];
                }
            }
        }
    }

    // Reset file pointer to the beginning
    rewind(file);

    // Process the code and output to function file
    FILE *FunctionFile = fopen(function_file_name, "w+");

    // Read the file character by character
    char c;
    char identifier[MAX_IDENTIFIER_LENGTH];
    int function_in_main[MAX_FUNC_COUNT];
    int function_count_in_main = 0;
    int identifierLength = 0;
    bool is_in_main = 0;
    while ((c = (char)fgetc(file)) != EOF)
    {
        // If the character is a brace, update the brace count
        if (c == '{')
        {
            braceCount++;
            fputs("{", FunctionFile);
        }
        else if (c == '}')
        {
            braceCount--;
            fputs("}", FunctionFile);
        }

        // If the character is a letter, it might be the start of an identifier
        else if (isalpha(c) || (identifierLength > 0 && (isalnum(c) || c == '_')))
        {
            // Add the character to the identifier
            identifier[identifierLength++] = c;
        }
        else
        {
            // If we have an identifier, process it
            if (identifierLength > 0)
            {
                // Null-terminate the identifier
                identifier[identifierLength] = '\0';

                if (braceCount == 0)
                {
                    // Check if the identifier is a function name
                    for (int i = 0; i < functionCount; i++)
                    {
                        if (strcmp(identifier, functionNames[i]) == 0)
                        {
                            if (strcmp(identifier, "main") == 0)
                            {
                                is_in_main = 1;
                            }
                            else
                            {
                                is_in_main = 0;
                            }
                            fputs("\n", FunctionFile);
                            fputs(identifier, FunctionFile);
                            fputs("\n", FunctionFile);
                            break;
                        }
                    }
                }
                else
                {
                    if (IsKeepWord(identifier))
                    {
                        fputs(identifier, FunctionFile);
                    }
                    else
                    {
                        // Check if the identifier is a function name
                        bool is_function_name = 0;
                        int i;
                        for (i = 0; i < functionCount; i++)
                        {
                            if (strcmp(identifier, functionNames[i]) == 0)
                            {
                                is_function_name = 1;
                                break;
                            }
                        }
                        if (is_function_name)
                        {
                            if (is_in_main)
                            {
                                // Add the function to the list of functions in main
                                int j;
                                for (j = 0; j < function_count_in_main; j++)
                                {
                                    if (function_in_main[j] == i)
                                    {
                                        break;
                                    }
                                }
                                if (j == function_count_in_main)
                                {
                                    function_in_main[function_count_in_main] = i;
                                    function_count_in_main++;
                                }
                            }
                            fputs("FUNC", FunctionFile);
                        }
                    }
                }

                // Reset the identifier length
                identifierLength = 0;
            }
            if (braceCount != 0)
            {
                // Remove all whitespace characters
                if (c != '\n' && c != '\r' && c != '\t' && c != ' ')
                {
                    fputc(c, FunctionFile);
                }
            }
        }
    }
    rewind(FunctionFile);

    // Process the code and output to a temporary file
    FILE *ProgramFile = fopen(program_file_name, "w");

    // Read the file line by line, and copy the main function to the program file first
    while (fgets(line, sizeof(line), FunctionFile))
    {
        if (strstr(line, "main"))
        {
            fgets(line, sizeof(line), FunctionFile);
            line[strcspn(line, "\n")] = 0; // Remove the newline character
            line[strcspn(line, "\r")] = 0; // Remove the carriage return character
            fputs(line, ProgramFile);
        }
    }

    // Then copy the other functions to the program file according to the order of function calls
    for (int i = 0; i < function_count_in_main; i++)
    {
        rewind(FunctionFile);
        while (fgets(line, sizeof(line), FunctionFile))
        {
            if (strstr(line, functionNames[function_in_main[i]]))
            {
                fgets(line, sizeof(line), FunctionFile);
                line[strcspn(line, "\n")] = 0; // Remove the newline character
                line[strcspn(line, "\r")] = 0; // Remove the carriage return character
                fputs(line, ProgramFile);
            }
        }
    }

    // Close the files
    fclose(file);
    fclose(FunctionFile);
    fclose(ProgramFile);
}

int program_classes[MAX_PROGRAM_COUNT][MAX_PROGRAM_COUNT] = {{0}}; // 2D array to keep track of which classes each program belongs to
int class_sizes[MAX_PROGRAM_COUNT] = {0};                          // Array to keep track of the size of each class

void CalculateSimilarityAndClassify()
{
    int i, j, k;
    int editDistance;
    double sim;
    unsigned int P1_len, P2_len;
    unsigned read_count;
    char P1_keyinfo[MAX_LINE_LENGTH], P2_keyinfo[MAX_LINE_LENGTH];
    char program_file_name[MAX_FILE_NAME_LENGTH];
    bool is_visited[MAX_PROGRAM_COUNT] = {false};

    for (i = 0; i < program_sum; i++)
    {
        if (is_visited[i])
        {
            continue;
        }
        // Load P1_keyinfo
        sprintf(program_file_name, "temp_program%s_program.c", program_id[i]);
        FILE *fp1 = fopen(program_file_name, "r");
        read_count = (unsigned int)fread(P1_keyinfo, sizeof(char), MAX_LINE_LENGTH, fp1);
        P1_keyinfo[read_count] = '\0'; // Add '\0' at the end
        fclose(fp1);
        P1_len = (unsigned int)strlen(P1_keyinfo);

        int class_size = 1;                          // Start with 1 because the program itself is in the class
        int class_programs[MAX_PROGRAM_COUNT] = {0}; // Array to keep track of programs in the current class
        class_programs[i] = 1;
        is_visited[i] = true;

        for (j = i + 1; j < program_sum; j++)
        {
            // Load P2_keyinfo
            sprintf(program_file_name, "temp_program%s_program.c", program_id[j]);
            FILE *fp2 = fopen(program_file_name, "r");
            read_count = (unsigned int)fread(P2_keyinfo, sizeof(char), MAX_LINE_LENGTH, fp2);
            P2_keyinfo[read_count] = '\0'; // Add '\0' at the end
            fclose(fp2);
            P2_len = (unsigned int)strlen(P2_keyinfo);

            // Calculate edit distance
            editDistance = EditdistDP(P1_keyinfo, P2_keyinfo);

            // Calculate similarity
            sim = 1 - (double)editDistance / max2(P1_len, P2_len);

            // Classify programs
            if (sim > THRESHOLD)
            {
                class_programs[j] = 1;
                class_size++;
                is_visited[j] = true;
            }
        }

        // Store the class and its size
        for (j = 0; j < program_sum; j++)
        {
            program_classes[i][j] = class_programs[j];
        }
        class_sizes[i] = class_size;
    }

    // Output classes
    for (i = 0; i < program_sum; i++)
    {
        // Check if the class is a subset of any previous class
        int is_subset = 0;
        for (j = 0; j < i; j++)
        {
            if (class_sizes[j] < class_sizes[i]) // If the previous class is smaller, skip it
            {
                continue;
            }

            is_subset = 1;
            for (k = 0; k < program_sum; k++)
            {
                if (program_classes[i][k] && !program_classes[j][k])
                {
                    is_subset = 0;
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
            for (j = 0; j < program_sum; j++)
            {
                if (program_classes[i][j])
                {
                    printf("%s ", program_id[j]);
                }
            }
            putchar('\n');
        }
    }
}

int **Dp;
long long unsigned int MaxDP = 3300; // for dynamic programming
int Min3(int a, int b, int c)
{
    int min = a < b ? a : b;
    return min < c ? min : c;
}

void Error2(const char *s)
{
    fprintf(stderr, "%s\n", s);
    exit(-1);
}

int InitDP()
{
    int i;
    Dp = (int **)malloc(MaxDP * sizeof(int *));
    for (i = 0; i < (int)MaxDP; i++)
    {
        Dp[i] = (int *)malloc(MaxDP * sizeof(int));
    }
    return 0;
}

int EditdistDP(char *str1, char *str2)
{
    int i, j;
    unsigned int len1, len2;
    static int flag = 0;

    if (flag++ == 0)
    {
        InitDP();
    }
    len1 = (unsigned int)strlen(str1) + 1;
    len2 = (unsigned int)strlen(str2) + 1;
    if (max2(len1, len2) >= MaxDP)
    {
        Error2("DP memory error!");
    }
    for (i = 0; i <= (int)len1; i++)
    {
        for (j = 0; j <= (int)len2; j++)
        {
            if (i == 0)
            {
                Dp[i][j] = j;
            }
            else if (j == 0)
            {
                Dp[i][j] = i;
            }
            else if (str1[i - 1] == str2[j - 1])
            {
                Dp[i][j] = Dp[i - 1][j - 1];
            }
            else
            {
                Dp[i][j] = 1 + Min3(Dp[i][j - 1], Dp[i - 1][j], Dp[i - 1][j - 1]);
            }
        }
    }
    return Dp[len1][len2];
}
