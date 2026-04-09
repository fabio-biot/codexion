#include "codexion.h"


pthread_mutex_t mutex;

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

    while (1)
    {
        pthread_mutex_lock(&sim->stop_mutex);
        if (sim->stop)
        {
            pthread_mutex_unlock(&sim->stop_mutex);
            break;
        }
        pthread_mutex_unlock(&sim->stop_mutex);
        if (coder->id % 2 == 0)
        {
            pthread_mutex_lock(&coder->right->mutex);
            print_state(sim, coder, "has taken a dongle", sim->start_time);

            pthread_mutex_lock(&coder->left->mutex);
            print_state(sim, coder, "has taken a dongle", sim->start_time);
        }
        else
        {
            pthread_mutex_lock(&coder->left->mutex);
            print_state(sim, coder, "has taken a dongle", sim->start_time);

            pthread_mutex_lock(&coder->right->mutex);
            print_state(sim, coder, "has taken a dongle", sim->start_time);
        }

        coder->last_compile = get_time_in_ms();
        print_state(sim, coder, "is compiling", sim->start_time);
        usleep(sim->time_to_compile * 1000);

        pthread_mutex_unlock(&coder->right->mutex);
        pthread_mutex_unlock(&coder->left->mutex);

        print_state(sim, coder, "is debugging", sim->start_time);
        usleep(sim->time_to_debug * 1000);

        print_state(sim, coder, "is refactoring", sim->start_time);
        usleep(sim->time_to_refactor * 1000);

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
    if (argc != 8 || !args)
    {
        printf("Failed to parse args.\n");
        return (1);
    }

    sim = init_sim(argc, args);
    sim->start_time = get_time_in_ms();
    sim->stop = 0;

    while (i < sim->number_of_coders)
    {
        pthread_create(&sim->coders[i].thread,
                       NULL,
                       coder_routine,
                       &sim->coders[i]);
        i++;
    }
    i = 0;
    while (i < sim->number_of_coders)
    {
        pthread_join(sim->coders[i].thread, NULL);
        i++;
    }
    return (0);
}