#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <stdbool.h>
#include <limits.h>

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define ABS(a) ((a) > 0 ? (a) : -(a))
#define IS_ODD(a) ((a) & 1)
#define IS_EVEN(a) !((a) & 1)

struct student
{
    int studentID;
    char name[50];
    int machineID;
    int loginTime;
};

int compareStudents(const void *a, const void *b)
{
    struct student *studentA = (struct student *)a;
    struct student *studentB = (struct student *)b;
    return studentA->studentID - studentB->studentID;
}

int main()
{
    struct student students[300];
    struct student abnormalStudents[300];
    int n;
    int sum = 0;
    int abnormalSum = 0;
    scanf("%d", &n);
    for (int i = 0; i < n; i++)
    {
        scanf("%d %s %d %d", &students[sum].studentID, students[sum].name, &students[sum].machineID, &students[sum].loginTime);
        sum++;
    }
    for (int i = 0; i < n; i++)
    {
        for (int j = i + 1; j < n; j++)
        {
            if (strcmp(students[i].name, students[j].name) == 0 && students[i].machineID != students[j].machineID)
            {
                // add to abnormalStudents
                int k;
                for (k = 0; k < abnormalSum; k++)
                {
                    if (students[i].studentID == abnormalStudents[k].studentID)
                    {
                        break;
                    }
                }
                if (k == abnormalSum)
                {
                    strcpy(abnormalStudents[k].name, students[i].name);
                    abnormalStudents[k].studentID = students[i].studentID;
                    abnormalSum++;
                }
            }
        }
    }

    for (int i = 0; i < abnormalSum; i++)
    {
        for (int j = i; j < abnormalSum; j++)
        {
            if (abnormalStudents[i].studentID > abnormalStudents[j].studentID)
            {
                int temp = abnormalStudents[i].studentID;
                abnormalStudents[i].studentID = abnormalStudents[j].studentID;
                abnormalStudents[j].studentID = temp;
                char tempName[100];
                strcpy(tempName, abnormalStudents[i].name);
                strcpy(abnormalStudents[i].name, abnormalStudents[j].name);
                strcpy(abnormalStudents[j].name, tempName);
            }
        }
    }

    qsort(abnormalStudents, abnormalSum, sizeof(struct student), compareStudents);

    for (int i = 0; i < abnormalSum; i++)
    {
        printf("%d %s\n", abnormalStudents[i].studentID, abnormalStudents[i].name);
    }

    return 0;
}
