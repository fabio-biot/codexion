#include "codexion.h"
#include <stdio.h>
#include <stdlib.h>


int parsing_args(int argc, char *argv[]) {
    printf("Nombre d args: %d\n", argc);

    if (argc != 8) {
        printf("Usage: %s <number_of_coders> <time_to_burnout>", argv[0]);
        printf("<time_to_compile> <time_to_debug> ");
        printf("<time_to_refactor> <number_of_compiles_required>");
        printf("<dongle_cooldown>\n");
        return 1;
    }

    printf("number_of_coders: %d\n", atoi(argv[1]));
    printf("time_to_burnout: %d\n", atoi(argv[2]));
    printf("time_to_compile: %d\n", atoi(argv[3]));
    printf("time_to_debug: %d\n", atoi(argv[4]));
    printf("time_to_refactor: %d\n", atoi(argv[5]));
    printf("number_of_compiles_required: %d\n", atoi(argv[6]));
    printf("dongle_cooldown: %d\n", atoi(argv[7]));
    return 0;
}
/*
number_of_coders
time_to_burnout
time_to_compile
time_to_debug
time_to_refactor
number_of_compiles_required
dongle_cooldown scheduler
*/
void *thread_print_five(void *arg)
{
    for (int i = 0; i < 5; i++) {
        printf("Thread: %s\n", (char*)arg);
    }
    return NULL;
}

void *thread_test(void *arg) {
    pthread_t thread1, thread2;

    pthread_create(&thread1, NULL, thread_print_five, "Thread 1");
    pthread_create(&thread2, NULL, thread_print_five, "Thread 2");

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    return NULL;
}


int main(int argc, char *argv[]) {
    printf("Hello, World!\n");
    parsing_args(argc, argv);
    thread_test(NULL);
    return 0;
}