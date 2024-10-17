#include <stdio.h>
#include <stdbool.h>

#define MAX_N 10

void generate_permutations(int nums[], bool used[], int permutation[], int N, int depth)
{
    if (depth == N)
    {
        for (int i = 0; i < N; i++)
        {
            printf("%d ", permutation[i]);
        }
        printf("\n");
        return;
    }

    for (int i = 0; i < N; i++)
    {
        if (!used[i])
        {
            used[i] = true;
            permutation[depth] = nums[i];
            generate_permutations(nums, used, permutation, N, depth + 1);
            used[i] = false;
        }
    }
}

int main()
{
    int N;
    scanf("%d", &N);

    int nums[MAX_N];
    for (int i = 0; i < N; i++)
    {
        nums[i] = i + 1;
    }

    bool used[MAX_N] = {false};
    int permutation[MAX_N];

    generate_permutations(nums, used, permutation, N, 0);

    return 0;
}
