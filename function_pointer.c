#include <stdio.h>

struct a
{
    int k;
    char c;
    char *e;
    int *t;
    void (*fn)(int);
};

void count(void (*function)(int), void (*function1)(int))
{
    for (int i = 0; i < 100; i++)
    {
        function(i);
        function1(i);
    }
}

void enes(int i)
{
    printf("zor %d\n", i);
}

void talha(int i)
{
    printf("bora %d\n", i);
}

int main(void)
{
    // count(&talha, &enes);
    struct a a;
    a.fn = &enes;
    a.fn(2);

    return 0;
}