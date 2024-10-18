/*
 * benchmark.c - A benchmark program for various Levenshtein algorithms
 *
 * How to run a perf test:
 *
 *   $ cc -o benchmark benchmark.c
 *   $ ./benchmark
 *
 * This program is placed in the public domain.
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <sys/time.h>

#define MIN(a,b) ((a) < (b) ? (a) : (b))
#define MAX(a,b) ((a) > (b) ? (a) : (b))

/*
 * Wagner-Fischer
 */
int wagner_fischer(char *s1, char *s2)
{
    int i, j, res, *buf;
    int len1, len2, left, top, dia, pos;

    len1 = strlen(s1);
    len2 = strlen(s2);

    buf = malloc((len1 + 1) * (len2 + 1) * sizeof(int));
    if (buf == NULL)
        return -1;

    for (j = 0; j <= len2; j++)
        buf[j] = j;

    pos = len2 + 1;
    for (i = 1; i <= len1; i++) {
        buf[pos++] = i;
        for (j = 1; j <= len2; j++) {
            if (s1[i - 1] == s2[j - 1]) {
                dia = buf[pos - len2 - 2];
                buf[pos++] = dia;
            } else {
                left = buf[pos - 1];
                top = buf[pos - len2 - 1];
                dia = buf[pos - len2 - 2];
                buf[pos++] = MIN(MIN(left, top), dia) + 1;
            }
        }
    }
    res = buf[pos - 1];
    free(buf);
    return res;
}

/*
 * Wagner-Fischer (Memory Reduction)
 */
int wagner_fischer_O1(char *s1, char *s2)
{
    int i, j, dia, tmp, *buf;
    int len1, len2;

    len1 = strlen(s1);
    len2 = strlen(s2);

    buf = malloc((len2 + 1) * sizeof(int));
    if (buf == NULL)
        return -1;

    for (j = 0; j <= len2; j++)
        buf[j] = j;

    for (i = 1; i <= len1; i++) {
        dia = i - 1;
        buf[0] = i;

        for (j = 1; j <= len2; j++) {
            tmp = buf[j];

            if (s1[i - 1] == s2[j - 1]) {
                buf[j] = dia;
            } else {
                buf[j] = MIN(MIN(buf[j], buf[j - 1]), dia) + 1;
            }
            dia = tmp;
        }
    }
    dia = buf[len2];
    free(buf);
    return dia;
}

/*
 * Wagner-Fischer (Memory Reduction + Branch Pruning)
 */
int wagner_fischer_O2(char *s1, char *s2)
{
    int i, j, m, M, len1, len2;
    int Mx, mx, dia, top, left, *buf;
    char *p1;

    len1 = strlen(s1);
    len2 = strlen(s2);
    if (len1 < len2)
        return wagner_fischer_O2(s2, s1);
    if (len2 == 0)
        return len1;
    if (len2 == 1)
        return len1 - (memchr(s1, s2[0], len1) != NULL);
    if (len2 == 2) {
        p1 = memchr(s1, s2[0], len1 - 1);
        if (p1) {
            return len1 - (index(p1+1, s2[1]) != NULL) - 1;
        } else {
            return len1 - (index(s1+1, s2[1]) != NULL);
        }
    }

    buf = malloc((len2 + 1) * sizeof(int));
    if (buf == NULL)
        return -1;

    Mx = (len2 - 1) / 2;
    mx = 1 - Mx - (len1 - len2);

    for (j = 0; j <= Mx; j++)
        buf[j] = j;

    for (i = 1; i <= len1; i++) {
        buf[0] = i - 1;

        m = MAX(mx, 1);
        M = MIN(Mx, len2);
        mx++;
        Mx++;

        dia = buf[m - 1];
        top = buf[m];

        if (s1[i - 1] != s2[m - 1])
            dia = MIN(dia, top) + 1;

        buf[m] = dia;
        left = dia;
        dia = top;

        for (j = m + 1; j <= M; j++) {
            top = buf[j];

            if (s1[i - 1] != s2[j - 1])
                dia = MIN(MIN(dia, top), left) + 1;
            buf[j] = dia;
            left = dia;
            dia = top;
        }

        if (len2 == M)
            continue;

        if (s1[i - 1] != s2[M])
            dia = MIN(dia, left) + 1;
        buf[M + 1] = dia;
    }
    dia = buf[len2];
    free(buf);
    return dia;
}

/*
 * Perf Test
 */
void int_to_bin(int num, char *buf)
{
    const char bin[] = "01";
    while (num >> 1) {
        *buf++ = bin[num % 2];
        num = num >> 1;
    }
    *buf = '\0';
}

double now(void)
{
  struct timeval t;
  gettimeofday(&t, (struct timezone *) 0);
  return t.tv_sec + t.tv_usec / (double) (1000000);
}

void bench(char *name, int (*func)(char*, char*))
{
    char s1[15];
    char s2[15];
    int i, j;
    int loop = 2048;
    double t0, dt;

    t0 = now();
    for (i = 1; i < loop; i++) {
        int_to_bin(i, s1);
        for (j = 1; j < loop; j++) {
            int_to_bin(j, s2);
            func(s1, s2);
        }
    }
    dt = now() - t0;
    printf("%-20s %10.3f %15.0f\n", name, dt, (loop * loop) / dt);
}

int main()
{
    printf("%-20s %10s %15s\n", "#", "TIME[s]", "SPEED[calls/s]");
    bench("wagner_fischer", wagner_fischer);
    bench("wagner_fischer_O1", wagner_fischer_O1);
    bench("wagner_fischer_O2", wagner_fischer_O2);
}
