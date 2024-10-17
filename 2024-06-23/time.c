#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_SIZE 10001

struct Student
{
    int id;
    int exerciseTime;
    int exerciseCount;
};

int compare(const void *a, const void *b)
{
    struct Student *studentA = (struct Student *)a;
    struct Student *studentB = (struct Student *)b;
    return studentB->exerciseTime - studentA->exerciseTime;
}

int main()
{
    struct Student BStudents[MAX_SIZE];
    struct Student MStudents[MAX_SIZE];
    struct Student DStudents[MAX_SIZE];
    int sum;
    int BStudentSum = 0, MStudentSum = 0, DStudentSum = 0;
    int studentID;
    scanf("%d", &sum);
    getchar();
    for (int i = 0; i < sum; i++)
    {
        int startHour, startMinute, startSecond, endHour, endMinute, endSecond;
        char type;
        scanf("%c%d %d %d %d %d %d %d", &type, &studentID, &startHour, &startMinute, &startSecond, &endHour, &endMinute, &endSecond);
        getchar();
        int period = 3600 * (endHour - startHour) + 60 * (endMinute - startMinute) + (endSecond - startSecond);
        bool isFound = false;
        if (type == 'B')
        {
            for (int j = 0; j < BStudentSum; j++)
            {
                if (BStudents[j].id == studentID)
                {
                    BStudents[j].exerciseTime += period;
                    BStudents[j].exerciseCount++;
                    isFound = true;
                    break;
                }
            }
            if (!isFound)
            {
                BStudents[BStudentSum].id = studentID;
                BStudents[BStudentSum].exerciseTime += period;
                BStudents[BStudentSum].exerciseCount = 1;
                BStudentSum++;
            }
        }
        else if (type == 'M')
        {
            for (int j = 0; j < MStudentSum; j++)
            {
                if (MStudents[j].id == studentID)
                {
                    MStudents[j].exerciseTime += period;
                    MStudents[j].exerciseCount++;
                    isFound = true;
                    break;
                }
            }
            if (!isFound)
            {
                MStudents[MStudentSum].id = studentID;
                MStudents[MStudentSum].exerciseTime += period;
                MStudents[MStudentSum].exerciseCount = 1;
                MStudentSum++;
            }
        }
        else if (type == 'D')
        {
            for (int j = 0; j < DStudentSum; j++)
            {
                if (DStudents[j].id == studentID)
                {
                    DStudents[j].exerciseTime += period;
                    DStudents[j].exerciseCount++;
                    isFound = true;
                    break;
                }
            }
            if (!isFound)
            {
                DStudents[DStudentSum].id = studentID;
                DStudents[DStudentSum].exerciseTime += period;
                DStudents[DStudentSum].exerciseCount = 1;
                DStudentSum++;
            }
        }
    }

    qsort(BStudents, BStudentSum, sizeof(BStudents[0]), compare);
    qsort(MStudents, MStudentSum, sizeof(MStudents[0]), compare);
    qsort(DStudents, DStudentSum, sizeof(DStudents[0]), compare);

    for (int i = 0; i < BStudentSum; i++)
    {
        printf("B%d %d %.2f\n", BStudents[i].id, BStudents[i].exerciseCount, (float)BStudents[i].exerciseTime / 3600);
    }
    for (int i = 0; i < MStudentSum; i++)
    {
        printf("M%d %d %.2f\n", MStudents[i].id, MStudents[i].exerciseCount, (float)MStudents[i].exerciseTime / 3600);
    }
    for (int i = 0; i < DStudentSum; i++)
    {
        printf("D%d %d %.2f\n", DStudents[i].id, DStudents[i].exerciseCount, (float)DStudents[i].exerciseTime / 3600);
    }

    return 0;
}
