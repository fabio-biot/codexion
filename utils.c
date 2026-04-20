#include "codexion.h"

int is_number(char *str)
{
    int i;

    i = 0;
    if (!str || str[0] == '\0')
        return (0);

    while (str[i])
    {
        if (str[i] < '0' || str[i] > '9')
            return (0);
        i++;
    }
    return (1);
}
// void wait(long coder_availiable_at, t_simulation *sim)
// {
//     while (get_time_in_ms() < coder_availiable_at)
//     {
//         if (sim->stop)
//             return ;
//         usleep(100);
//     }
// }

int dongle_available(t_dongle *d)
{
    long now;

    now = get_time_in_ms();

    if (d->is_taken)
    {
        printf("cas");
        return (0);
    }
    if (now < d->available_at)
    {
        printf("cas");
        return (0);
    }
    printf("\nCheck dongle availiable\n");
    return (1);
}

t_request *create_request(t_simulation *sim, t_coder *coder)
{
    t_request *req;

    req = malloc(sizeof(t_request));
    if (!req)
        return (NULL);

    req->coder_id = coder->id;
    req->arrival_time = get_time_in_ms();

    pthread_mutex_lock(&coder->lock);
    req->deadline = coder->last_compile_start + sim->time_to_burnout;
    pthread_mutex_unlock(&coder->lock);
    printf("Request created\n");
    return (req);
}

void request_dongle(t_simulation *sim, t_coder *coder, t_dongle *d)
{
    t_request *req;

    pthread_mutex_lock(&d->mutex);
    if (sim->stop)
    {
        pthread_mutex_unlock(&d->mutex);
        return;
    }
    req = create_request(sim, coder);
    if (!req)
    {
        pthread_mutex_unlock(&d->mutex);
        return;
    }
    push_heap(d, req, sim->scheduler);
    while (1)
    {
        if (sim->stop)
        {
            pthread_mutex_unlock(&d->mutex);
            return;
        }
        if (d->heap[0] == req && !d->is_taken && get_time_in_ms() >= d->available_at)
        {
            pop_heap(d, sim->scheduler);
            d->is_taken = 1;
            pthread_mutex_unlock(&d->mutex);
            return;
        }
        pthread_cond_wait(&d->cond, &d->mutex);
    }
}

int get_stop(t_simulation *sim)
{
    int stop;

    pthread_mutex_lock(&sim->stop_mutex);
    stop = sim->stop;
    pthread_mutex_unlock(&sim->stop_mutex);
    return (stop);
}
