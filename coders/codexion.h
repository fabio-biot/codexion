
#ifndef FT_CODEXION_H
#define FT_CODEXION_H

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/time.h>

typedef struct s_request
{
    int coder_id;
    long arrival_time;
    long deadline;
} t_request;

typedef struct s_dongle
{
    pthread_mutex_t mutex;
    pthread_cond_t cond;

    t_request **heap;
    int size;
    int capacity;

    long available_at;
    int is_taken;
    struct s_simulation *sim;
} t_dongle;

typedef struct s_coder
{
    int id;
    int compile_count;
    long last_compile_start;

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
    int coders_ended;
    char* scheduler;

    t_dongle *dongles;
    t_coder *coders;

    pthread_t thread_monitor;
    pthread_mutex_t print_mutex;
    pthread_mutex_t stop_mutex;
} t_simulation;

t_simulation *init_sim(int* args, char* scheduler);
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
t_request *init_request();
t_request *create_request(t_simulation *sim, t_coder *coder);
void request_dongle(t_simulation *sim, t_coder *coder, t_dongle *dongle);
int compare(t_request *a, t_request *b, char *scheduler);
void swap(t_request **a, t_request **b);
void heapify_up(t_dongle *d, int index, char *scheduler);
void heapify_down(t_dongle *d, int index, char *scheduler);
t_request *pop_heap(t_dongle *d, char *scheduler);
int push_heap(t_dongle *d, t_request *req, char *scheduler);
void remove_request(t_dongle *d, t_request *req, char *scheduler);
int dongle_available(t_dongle *d);
void *process_take_dongle(t_coder *coder, t_simulation *sim, t_dongle *dongle);
void release_dongle(t_dongle *d);
int get_stop(t_simulation *sim);

#endif