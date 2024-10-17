#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>

#define MAX_PERSON 100
#define Min(a, b) ((a) < (b) ? (a) : (b))
#define Max(a, b) ((a) > (b) ? (a) : (b))

struct person
{
    unsigned int ID;
    char name[30];
    int seat;
};

bool seat[MAX_PERSON] = {false};

struct person persons[MAX_PERSON];

int CompareFirst(const void *a, const void *b);
int CompareSecond(const void *a, const void *b);

int CompareFirst(const void *a, const void *b)
{
    struct person *pa = (struct person *)a;
    struct person *pb = (struct person *)b;

    if (pa->seat < pb->seat)
    {
        return -1;
    }
    else if (pa->seat > pb->seat)
    {
        return 1;
    }
    else
    {
        return pa->ID > pb->ID ? 1 : -1;
    }
}

int CompareSecond(const void *a, const void *b)
{
    struct person *pa = (struct person *)a;
    struct person *pb = (struct person *)b;

    return pa->ID > pb->ID ? 1 : -1;
}

int main()
{
    FILE *fp = fopen("in.txt", "r");
    int sum = 0;
    int max_seat = INT_MIN;

    // Read the first line
    while (fscanf(fp, "%u %s %d", &persons[sum].ID, persons[sum].name, &persons[sum].seat) != EOF)
    {
        seat[persons[sum].seat] = true;
        max_seat = Max(max_seat, persons[sum].seat);
        sum++;
    }
    fclose(fp);

    qsort(persons, (size_t)sum, sizeof(struct person), CompareFirst);

    for (int i = 0, j = sum; i <= Min(max_seat, sum); i++)
    {
        if (seat[i] == false)
        {
            persons[j].seat = i;
            j--;
        }
        max_seat = INT_MIN;
        for (int k = 0; k < sum; k++)
        {
            max_seat = Max(max_seat, persons[k].seat);
        }
    }

    // check if the seat is duplicated
    for (int i = 0; i < sum; i++)
    {
        for (int j = i + 1; j < sum; j++)
        {
            if (persons[i].seat == persons[j].seat)
            {
                if (persons[i].ID > persons[j].ID)
                {
                    persons[i].seat = ++max_seat;
                }
                else
                {
                    persons[j].seat = ++max_seat;
                }
            }
        }
    }

    qsort(persons, (size_t)sum, sizeof(struct person), CompareSecond);

    FILE *out = fopen("out.txt", "w");
    for (int i = 0; i < sum; i++)
    {
        fprintf(out, "%u %s %d\n", persons[i].ID, persons[i].name, persons[i].seat);
    }

    return 0;
}
