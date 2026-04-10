
#ifndef FT_CODEXION_H
#define FT_CODEXION_H

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/time.h>


typedef struct s_dongle
{
    pthread_mutex_t mutex;
    long last_used;
    long available_at;
} t_dongle;

typedef struct s_coder
{
    int id;
    int compile_count;
    long last_compile;

    pthread_t thread;
    pthread_mutex_t lock;

    t_dongle *left;
    t_dongle *right;

    struct s_simulation *sim;
} t_coder;

typedef struct s_simulation
{
    int number_of_coders;

    long time_to_burnout;
    long time_to_compile;
    long time_to_debug;
    long time_to_refactor;
    int number_of_compiles_required;
    long dongle_cooldown;
    int stop;
    long start_time;

    t_dongle *dongles;
    t_coder *coders;

    pthread_t thread_monitor;
    pthread_mutex_t print_mutex;
    pthread_mutex_t stop_mutex;
} t_simulation;

t_simulation *init_sim(int* args);
int *parsing_args(int argc, char *argv[]);
void *thread_test(void *arg);
t_coder *init_coders(t_simulation *sim);
t_dongle *init_dongles(t_simulation *sim);
void *thread_print_five(void *arg);
void *thread_print_five_sec(void *arg);
long get_time_in_ms();
int is_number(char *str);
void print_state(t_simulation *sim, t_coder *coder, char *state, long start_time);
void *monitor_thread(void *arg);


#endif