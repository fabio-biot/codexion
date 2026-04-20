#include "codexion.h"


long get_time_in_ms(void)
{
    struct timeval tv;

    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000L + tv.tv_usec / 1000L);
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
    while (sim->number_of_compiles_required > i)
    {
        if (get_stop(sim))
            return NULL;
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
        process_take_dongle(coder, sim, second);        
        printf("process take dongles done for coder %d\n", coder->id);
        i++;
        coder->compile_count++;
    }
    return NULL;
}

int main(int argc, char *argv[])
{
    int *args;
    int i;
    t_simulation *sim;

    args = parsing_args(argc, argv);
    i = 0;
    if (argc != 9 || !args)
    {
        printf("Failed to parse args.\n");
        return (1);
    }

    sim = init_sim(args, argv[8]);
    sim->start_time = get_time_in_ms();
    sim->stop = 0;

    while (i < sim->number_of_coders)
    {
        pthread_create(&sim->coders[i].thread,
                       NULL,
                       coder_routine,
                       &sim->coders[i]);
        printf("Thread created for coder %d\n", i);
        i++;
    }
    pthread_create(&sim->thread_monitor, NULL, monitor_thread, sim);
    i = 0;
    while(i < sim->number_of_coders)
    {
        pthread_join(sim->coders[i].thread, NULL);
        i++;
    }
    pthread_join(sim->thread_monitor, NULL);

    return (0);
}