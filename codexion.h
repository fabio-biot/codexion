
#ifndef FT_CODEXION_H
#define FT_CODEXION_H

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>


typedef struct s_dongle
{
    pthread_mutex_t mutex;
    long last_used;
} t_dongle;

typedef struct s_coder
{
    int id;
    int compile_count;
    long last_compile;

    pthread_t thread;

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

    t_dongle *dongles;
    t_coder *coders;

    pthread_mutex_t print_mutex;
    pthread_mutex_t stop_mutex;
} t_simulation;

#endif