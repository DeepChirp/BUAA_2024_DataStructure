#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>

#define MAX_SIZE 100

char frontStack[MAX_SIZE][MAX_SIZE];
char backStack[MAX_SIZE][MAX_SIZE];
int frontTop = -1;
int backTop = -1;

void FrontPush(char *s)
{
    if (frontTop == MAX_SIZE - 1)
    {
        printf("Error: stack overflow\n");
        return;
    }
    strcpy(frontStack[++frontTop], s);
}

bool IsFrontStackEmpty()
{
    return frontTop == -1;
}

void FrontPop()
{
    frontTop--;
}

void BackPush(char *s)
{
    if (backTop == MAX_SIZE - 1)
    {
        printf("Error: stack overflow\n");
        return;
    }
    strcpy(backStack[++backTop], s);
}

bool IsBackStackEmpty()
{
    return backTop == -1;
}

void BackPop()
{
    backTop--;
}

int main()
{
    char current[MAX_SIZE], domain[MAX_SIZE];
    char visitDomain[MAX_SIZE][MAX_SIZE];
    char command[MAX_SIZE];
    int visitTime[MAX_SIZE];
    int domainSum = 0;
    bool shouldSkip = false;
    bool noExtraOutput;
    strcpy(current, "https://www.baidu.com/");
    while (1)
    {
        if (!shouldSkip)
        {
            printf("%s\n", current);
            sscanf(current, "https://%s/", domain);
            strstr(domain, "/") ? *strstr(domain, "/") = '\0' : 0;
            bool isVisited = false;
            for (int i = 0; i < domainSum; i++)
            {
                if (strcmp(visitDomain[i], domain) == 0)
                {
                    visitTime[i]++;
                    isVisited = true;
                    break;
                }
            }
            if (!isVisited)
            {
                strcpy(visitDomain[domainSum], domain);
                visitTime[domainSum]++;
                domainSum++;
            }
        }
        shouldSkip = false;

        scanf("%s", command);
        if (strcmp(command, "VISIT") == 0)
        {
            BackPush(current);
            frontTop = -1;
            scanf("%s", current);
        }
        else if (strcmp(command, "<<") == 0)
        {
            if (!IsBackStackEmpty())
            {
                FrontPush(current);
                strcpy(current, backStack[backTop]);
                BackPop();
            }
            else
            {
                shouldSkip = true;
            }
        }
        else if (strcmp(command, ">>") == 0)
        {
            if (!IsFrontStackEmpty())
            {
                BackPush(current);
                strcpy(current, frontStack[frontTop]);
                FrontPop();
            }
            else
            {
                shouldSkip = true;
            }
        }
        else if (strcmp(command, "QUIT") == 0)
        {
            scanf("%d", (int *)&noExtraOutput);
            break;
        }
    }

    if (!noExtraOutput)
    {
        int maxDomainIndex = -1;
        int currentMax = INT_MIN;
        for (int i = 0; i < domainSum; i++)
        {
            if (visitTime[i] > currentMax)
            {
                maxDomainIndex = i;
                currentMax = visitTime[i];
            }
        }
        printf("%s %d", visitDomain[maxDomainIndex], visitTime[maxDomainIndex]);
    }

    return 0;
}
