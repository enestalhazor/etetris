#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

int increased = 0;
int a = 10;
pthread_mutex_t m;

void *increase()
{
    for (int i = 0; i < 100; i++)
    {
        pthread_mutex_lock(&m);
        if (increased == 1)
        {
            pthread_mutex_unlock(&m);
            continue;
        }

        a++;
        printf("%d\n", a);
        increased = 1;
        pthread_mutex_unlock(&m);
    }

    return NULL;
}

void *decrease()
{
    for (int i = 0; i < 100; i++)
    {
        pthread_mutex_lock(&m);
        if (increased == 0)
        {
            pthread_mutex_unlock(&m);
            continue;
        }

        a--;
        printf("%d\n", a);
        increased = 0;
        pthread_mutex_unlock(&m);
    }

    return NULL;
}

int main(void)
{
    pthread_t t1;
    pthread_t t2;
    pthread_create(&t1, NULL, &increase, NULL);
    pthread_create(&t2, NULL, &decrease, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("%d\n", a);

    return 0;
}