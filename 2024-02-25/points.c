#include <stdio.h>

typedef struct
{
    int x, y;
} Point;

double area(Point *points, int n)
{
    double area = 0;
    for (int i = 1; i < n - 1; i++)
    {
        int x1 = points[i].x - points[0].x;
        int y1 = points[i].y - points[0].y;
        int x2 = points[i + 1].x - points[0].x;
        int y2 = points[i + 1].y - points[0].y;
        // 三角形面积公式
        area += (double)(x1 * y2 - x2 * y1) / 2;
    }
    return area < 0 ? -area : area;
}

int main()
{
    int n;
    scanf("%d", &n);
    Point points[n];
    for (int i = 0; i < n; i++)
    {
        scanf("%d %d", &points[i].x, &points[i].y);
    }
    printf("%.2f\n", area(points, n));
    return 0;
}