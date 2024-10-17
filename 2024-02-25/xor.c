#include <stdio.h>
#include <string.h>

// 插入排序
void insertSort(int arr[], int num)
{
    for (int i = 1; i < num; i++)
    {
        int temp = arr[i], j = i - 1;
        while (j >= 0 && arr[j] < temp)
        {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = temp;
    }
}

int main()
{
    int arr1[30] = {0}, arr2[30] = {0}, num1 = 0, num2 = 0;

    char line[256];
    char *token;

    // 读取并解析第一行
    fgets(line, sizeof(line), stdin);
    token = strtok(line, " ");
    while (token != NULL)
    {
        sscanf(token, "%d", &arr1[num1++]);
        token = strtok(NULL, " ");
    }

    // 读取并解析第二行
    fgets(line, sizeof(line), stdin);
    token = strtok(line, " ");
    while (token != NULL)
    {
        sscanf(token, "%d", &arr2[num2++]);
        token = strtok(NULL, " ");
    }

    insertSort(arr1, num1);
    insertSort(arr2, num2);

    int i = 0, j = 0;
    while (i < num1 && j < num2)
    {
        if (arr1[i] == arr2[j])
        {
            i++;
            j++;
        }
        else if (arr1[i] > arr2[j])
        {
            printf("%d ", arr1[i]);
            i++;
        }
        else
        {
            printf("%d ", arr2[j]);
            j++;
        }
    }

    while (i < num1)
    {
        printf("%d ", arr1[i]);
        i++;
    }

    while (j < num2)
    {
        printf("%d ", arr2[j]);
        j++;
    }

    return 0;
}
