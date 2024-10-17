#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int selectSort(int *arr, int n);
int bubbleSort(int *arr, int n);
int heapSort(int *arr, int i, int n);
void mergeSort(int *arr, int *tmp, int left, int left_end, int right_end, int *times);
void quickSort(int k[], int left, int right, int *times);

// select sort
int selectSort(int *arr, int n)
{
    int i, j, minIndex, temp;
    int times = 0;
    for (i = 0; i < n - 1; i++)
    {
        minIndex = i;
        for (j = i + 1; j < n; j++)
        {
            times++;
            if (arr[j] < arr[minIndex])
            {
                minIndex = j;
            }
        }
        if (minIndex != i)
        {
            temp = arr[i];
            arr[i] = arr[minIndex];
            arr[minIndex] = temp;
        }
    }
    return times;
}

// bubble sort
int bubbleSort(int *arr, int n)
{
    int i, j, temp;
    int times = 0;
    for (i = 0; i < n - 1; i++)
    {
        bool flag = false; // flag for optimization
        for (j = 0; j < n - i - 1; j++)
        {
            times++;
            if (arr[j] > arr[j + 1])
            {
                temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
                flag = true;
            }
        }
        if (!flag)
        {
            break;
        }
    }
    return times;
}

// heap sort
int heapSort(int *arr, int i, int n)
{
    int j, temp;
    int times = 0;
    temp = arr[i];
    j = 2 * i + 1;
    while (j < n)
    {
        if (j < n - 1 && arr[j] < arr[j + 1])
        {
            j++;
        }
        times++;
        if (temp >= arr[j])
        {
            break;
        }
        arr[(j - 1) / 2] = arr[j];
        j = 2 * j + 1;
    }
    arr[(j - 1) / 2] = temp;
    return times;
}

// merge sort
void mergeSort(int *arr, int *tmp, int left, int left_end, int right_end, int *times)
{
    if (left < right_end)
    {
        int mid = left_end + 1;
        mergeSort(arr, tmp, left, (left + left_end) / 2, left_end, times);
        mergeSort(arr, tmp, mid, (mid + right_end) / 2, right_end, times);
        int i = left, j = left_end + 1, k = left;
        while (i <= left_end && j <= right_end)
        {
            (*times)++;
            if (arr[i] <= arr[j])
            {
                tmp[k++] = arr[i++];
            }
            else
            {
                tmp[k++] = arr[j++];
            }
        }
        while (i <= left_end)
        {
            tmp[k++] = arr[i++];
        }
        while (j <= right_end)
        {
            tmp[k++] = arr[j++];
        }
        for (i = left; i <= right_end; i++)
        {
            arr[i] = tmp[i];
        }
    }
}

// quick sort
void quickSort(int k[], int left, int right, int *times)
{
    int i, last, temp;
    if (left < right)
    {
        last = left;
        for (i = left + 1; i <= right; i++)
        {
            (*times)++;
            if (k[i] < k[left])
            {
                // swap(k[i], k[++last]);
                last++;
                temp = k[last];
                k[last] = k[i];
                k[i] = temp;
            }
        }
        // swap(k[left], k[last]);
        temp = k[left];
        k[left] = k[last];
        k[last] = temp;
        quickSort(k, left, last - 1, times);
        quickSort(k, last + 1, right, times);
    }
}

int main()
{
    int sum, method;
    scanf("%d %d", &sum, &method);
    int *arr = (int *)malloc(sizeof(int) * (unsigned long long)sum);
    int *tmp = (int *)malloc(sizeof(int) * (unsigned long long)sum);
    for (int i = 0; i < sum; i++)
    {
        scanf("%d", &arr[i]);
    }
    int times = 0;
    switch (method)
    {
    case 1:
        times = selectSort(arr, sum);
        break;
    case 2:
        times = bubbleSort(arr, sum);
        break;
    case 3:
        for (int i = sum / 2 - 1; i >= 0; i--)
        {
            times += heapSort(arr, i, sum);
        }
        for (int i = sum - 1; i > 0; i--)
        {
            int temp = arr[0];
            arr[0] = arr[i];
            arr[i] = temp;
            times += heapSort(arr, 0, i);
        }
        break;
    case 4:
        mergeSort(arr, tmp, 0, sum / 2 - 1, sum - 1, &times);
        free(tmp);
        break;
    case 5:
        quickSort(arr, 0, sum - 1, &times);
        break;
    }

    for (int i = 0; i < sum; i++)
    {
        printf("%d ", arr[i]);
    }
    putchar('\n');
    printf("%d\n", times);

    return 0;
}
