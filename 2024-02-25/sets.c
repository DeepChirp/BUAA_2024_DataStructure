#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 1000

int hash_table[MAX_SIZE] = {0};

int main()
{
    int num;
    int A[MAX_SIZE], n = 0;

    // Read set A
    while (scanf("%d", &num) && num != -1)
    {
        A[n++] = num;
    }

    // Read set B and store it in the hash table
    while (scanf("%d", &num) && num != -1)
    {
        hash_table[num] = 1;
    }

    // Print the difference set
    for (int i = 0; i < n; i++)
    {
        if (!hash_table[A[i]])
        {
            printf("%d ", A[i]);
        }
    }

    return 0;
}