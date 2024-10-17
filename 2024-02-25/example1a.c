#include <stdio.h>
#include <math.h>

int main()
{
    double e;
    scanf("%lf", &e);

    double pi = 1.0;
    double last_pi = 0.0;
    double numerator = 1.0;
    double denominator = 1.0;
    int n = 1;

    while (fabs(pi - last_pi) >= e / 2 || n < 2)
    {
        last_pi = pi;
        numerator *= n;
        denominator *= (2 * n + 1);
        pi += numerator / denominator;
        n++;
    }

    printf("%d %.7lf\n", n, pi * 2);

    return 0;
}