#include <stdio.h>
#include <math.h>

int min(int a, int b)
{
    return a < b ? a : b;
}

int max(int a, int b)
{
    return a > b ? a : b;
}

int main()
{
    int Ax1, Ay1, Ax2, Ay2;
    int Bx1, By1, Bx2, By2;
    scanf("%d%d%d%d", &Ax1, &Ay1, &Ax2, &Ay2);
    scanf("%d%d%d%d", &Bx1, &By1, &Bx2, &By2);

    int max_Ax = max(Ax1, Ax2);
    int min_Ax = min(Ax1, Ax2);
    int max_Bx = max(Bx1, Bx2);
    int min_Bx = min(Bx1, Bx2);
    int x = min(max_Ax, max_Bx) - max(min_Ax, min_Bx);

    int max_Ay = max(Ay1, Ay2);
    int min_Ay = min(Ay1, Ay2);
    int max_By = max(By1, By2);
    int min_By = min(By1, By2);
    int y = min(max_Ay, max_By) - max(min_Ay, min_By);

    printf("%d\n", max(0, x) * max(0, y));

    return 0;
}
