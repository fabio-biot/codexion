#include "codexion.h"
#include <unistd.h>

pthread_mutex_t mutex;

void *thread_print_five(void *arg)
{
    int i = 0;
    while (i < 10)
    {
        pthread_mutex_lock(&mutex);
        printf("1: %s\n", (char*)arg);
        pthread_mutex_unlock(&mutex);
        usleep(1000);
        i++;
    }
    return NULL;
}

void *thread_print_five_sec(void *arg)
{
    int i = 0;
    while (i < 10)
    {
        pthread_mutex_lock(&mutex);
        printf("2: %s\n", (char*)arg);
        pthread_mutex_unlock(&mutex);
        usleep(1000);
        i++;
    }
    return NULL;
}

void *thread_test(void *arg)
{
    pthread_t thread1, thread2;

    pthread_mutex_init(&mutex, NULL);

    pthread_create(&thread1, NULL, thread_print_five, "Thread 1");
    pthread_create(&thread2, NULL, thread_print_five_sec, "Thread 2");

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    pthread_mutex_destroy(&mutex);

    return NULL;
}

int main()
{
    pthread_t thread;

    pthread_create(&thread, NULL, thread_test, NULL);
    pthread_join(thread, NULL);

    return 0;
}