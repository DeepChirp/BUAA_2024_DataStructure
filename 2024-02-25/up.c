#include <stdio.h>
#include <string.h>

int main()
{
    char s[10005];
    scanf("%s", s);

    int length = strlen(s);
    int dp[10005];
    int max = 0;
    for (int i = 0; i < length; i++)
    {
        dp[i] = 1;
        for (int j = 0; j < i; j++)
        {
            if (s[i] >= s[j])
            {
                dp[i] = dp[i] > dp[j] + 1 ? dp[i] : dp[j] + 1;
            }
        }
        max = max > dp[i] ? max : dp[i];
    }
    printf("%d\n", max);

    return 0;
}