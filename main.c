#include "codexion.h"

long get_time_in_ms(void)
{
    struct timeval tv;

    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void print_state(t_simulation *sim, t_coder *coder, char *msg, long start)
{
    pthread_mutex_lock(&sim->print_mutex);
    printf("%ld %d %s\n", get_time_in_ms() - start, coder->id, msg);
    pthread_mutex_unlock(&sim->print_mutex);
}

void *coder_routine(void *arg)
{
    t_coder *coder = (t_coder *)arg;
    t_simulation *sim = coder->sim;
    t_dongle *first;
    t_dongle *second;
    int i;

    i = 0;
    while (!get_stop(sim) && i < sim->number_of_compiles_required)
    {
        if (coder->left < coder->right)
        {
            first = coder->left;
            second = coder->right;
        }
        else
        {
            first = coder->right;
            second = coder->left;
        }
        process_take_dongle(coder, sim, first);
        if (get_stop(sim))
            break;
        process_take_dongle(coder, sim, second);
        if (get_stop(sim))
            break;
        pthread_mutex_lock(&coder->lock);
        coder->last_compile_start = get_time_in_ms();
        pthread_mutex_unlock(&coder->lock);
        print_state(sim, coder, "is compiling", sim->start_time);
        usleep(sim->time_to_compile * 1000);
        if (get_stop(sim))
            break;
        release_dongle(first);
        release_dongle(second);
        coder->compile_count++;
        i++;
        print_state(sim, coder, "is debugging", sim->start_time);
        usleep(sim->time_to_debug * 1000);
        print_state(sim, coder, "is refactoring", sim->start_time);
        usleep(sim->time_to_refactor * 1000);
    }
    pthread_mutex_lock(&sim->stop_mutex);
    sim->coders_ended++;
    pthread_mutex_unlock(&sim->stop_mutex);
    return NULL;
}

void join_all(t_simulation *sim)
{
    int i = 0;

    while (i < sim->number_of_coders)
    {
        pthread_join(sim->coders[i].thread, NULL);
        i++;
    }
    pthread_join(sim->thread_monitor, NULL);
}

void cleanup_simulation(t_simulation *sim)
{
    int i;

    if (!sim)
        return;
    i = 0;
    while (i < sim->number_of_coders)
    {
        pthread_mutex_destroy(&sim->coders[i].lock);
        i++;
    }
    i = 0;
    while (i < sim->number_of_coders)
    {
        pthread_mutex_destroy(&sim->dongles[i].mutex);
        pthread_cond_destroy(&sim->dongles[i].cond);
        if (sim->dongles[i].heap)
            free(sim->dongles[i].heap);

        i++;
    }
    pthread_mutex_destroy(&sim->print_mutex);
    pthread_mutex_destroy(&sim->stop_mutex);
    if (sim->coders)
        free(sim->coders);
    if (sim->dongles)
        free(sim->dongles);
    free(sim);
}

int main(int argc, char *argv[])
{
    int *args;
    int i;
    t_simulation *sim;
    if (argc != 9)
    {
        printf("Invalid args\n");
        return 1;
    }
    args = parsing_args(argc, argv);
    if (!args)
    {
        printf("Failed to parse args\n");
        return 1;
    }
    sim = init_sim(args, argv[8]);
    sim->start_time = get_time_in_ms();
    sim->stop = 0;
    i = 0;
    while (i < sim->number_of_coders)
    {
        pthread_create(&sim->coders[i].thread, NULL,
                       coder_routine, &sim->coders[i]);
        i++;
    }
    pthread_create(&sim->thread_monitor, NULL, monitor_thread, sim);
    join_all(sim);
    cleanup_simulation(sim);
    return 0;
}