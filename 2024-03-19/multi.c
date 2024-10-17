#include <stdio.h>
#include <stdlib.h>

typedef struct PolyNode
{
    int coef;
    int expn;
    struct PolyNode *next;
} PolyNode, *Polynomial;

Polynomial createPolyNode(int coef, int expn)
{
    Polynomial p = (Polynomial)malloc(sizeof(PolyNode));
    p->coef = coef;
    p->expn = expn;
    p->next = NULL;
    return p;
}

void insertPolyNode(Polynomial *poly, int coef, int expn)
{
    Polynomial p = *poly;
    Polynomial q = createPolyNode(coef, expn);
    if (!*poly || (*poly)->expn < expn)
    {
        q->next = *poly;
        *poly = q;
    }
    else
    {
        while (p->next && p->next->expn >= expn)
            p = p->next;
        q->next = p->next;
        p->next = q;
    }
}

Polynomial multiplyPolynomials(Polynomial poly1, Polynomial poly2)
{
    Polynomial p = poly1, q, r;
    Polynomial poly = NULL;
    while (p)
    {
        q = poly2;
        while (q)
        {
            insertPolyNode(&poly, p->coef * q->coef, p->expn + q->expn);
            q = q->next;
        }
        p = p->next;
    }
    p = poly;
    while (p && p->next)
    {
        if (p->next->expn == p->expn)
        {
            r = p->next;
            p->coef += r->coef;
            p->next = r->next;
            free(r);
        }
        else
        {
            p = p->next;
        }
    }
    return poly;
}

int main()
{
    Polynomial poly1 = NULL, poly2 = NULL, poly;
    int coef, expn;
    char ch;
    do
    {                                       // 创建一个带头结点的有序链表存放第一个多项式
        scanf("%d%d%c", &coef, &expn, &ch); // 读入第一行系数和指数
        insertPolyNode(&poly1, coef, expn); // 插入到有序链表中
    } while (ch != '\n');
    while (scanf("%d%d", &coef, &expn) != EOF)
    {
        insertPolyNode(&poly2, coef, expn);
    }
    poly = multiplyPolynomials(poly1, poly2);
    while (poly)
    {
        printf("%d %d ", poly->coef, poly->expn);
        poly = poly->next;
    }
    printf("\n");
    return 0;
}