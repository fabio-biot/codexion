#include "codexion.h"

void *thread_print_five(void *arg)
{
    int i = 0;
    pthread_mutex_lock(&mutex);
    while(i < 1500)
    {
        printf("1: %s\n", (char*)arg);
        i++;
    }
    pthread_mutex_unlock(&mutex);
    return NULL;
}

void *thread_print_five_sec(void *arg)
{
    int i = 0;
    pthread_mutex_lock(&mutex);
    while(i < 1500)
    {
        printf("2: %s\n", (char*)arg);
        i++;
    }
    pthread_mutex_unlock(&mutex);
    return NULL;
}

void *thread_test(void *arg) {
    char *str = "Thread 1";
    pthread_t thread1, thread2;
    pthread_mutex_init(&mutex, NULL);
    pthread_create(&thread1, NULL, thread_print_five, str);
    pthread_create(&thread2, NULL, thread_print_five_sec, str);
    for (int i = 0; i < 10000005; i++);
    
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    return NULL;
}