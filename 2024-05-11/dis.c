#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MIN(a, b) ((a) < (b) ? (a) : (b))

int EditDistanceDP(char *str1, char *str2)
{
    int len1 = strlen(str1);
    int len2 = strlen(str2);
    int dp[2][len2 + 1]; // Use rolling window to reduce memory usage

    // Handle the case when i=0
    for (int j = 0; j <= len2; j++)
    {
        dp[0][j] = j;
    }

    // Handle the case when j=0
    for (int i = 1; i <= len1; i++)
    {
        dp[i % 2][0] = i;

        for (int j = 1; j <= len2; j++)
        {
            if (str1[i - 1] == str2[j - 1])
            {
                dp[i % 2][j] = dp[(i - 1) % 2][j - 1];
            }
            else
            {
                dp[i % 2][j] = 1 + MIN((MIN(dp[i % 2][j - 1], dp[(i - 1) % 2][j])), (dp[(i - 1) % 2][j - 1]));
            }
        }
    }
    return dp[len1 % 2][len2];
}

int main()
{
    char str[100][100];
    char difference[100];
    int n;
    int minDistance = INT_MAX, minIndex = -1;
    scanf("%d", &n);

    for (int i = 0; i < n; i++)
    {
        scanf("%s", str[i]);
    }

    for (int i = 1; i < n; i++)
    {
        int distance = EditDistanceDP(str[0], str[i]);
        if (minDistance > distance)
        {
            minDistance = distance;
            minIndex = i;
        }
    }

    int len1 = strlen(str[0]);
    int len2 = strlen(str[minIndex]);

    int i = 0;
    while (str[0][i] == str[minIndex][i])
    {
        i++;
    }
    int j = i;
    int k = 0;
    if (len1 > len2)
    {
        while (str[0][j] != str[minIndex][i])
        {
            difference[k++] = str[0][j++];
        }
    }
    else
    {
        while (str[0][i] != str[minIndex][j])
        {
            difference[k++] = str[minIndex][j++];
        }
    }

    printf("%s %s %d %s", str[0], str[minIndex], minDistance, difference);

    return 0;
}