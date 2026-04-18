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
void wait(long coder_availiable_at, t_simulation *sim)
{
    while (get_time_in_ms() < coder_availiable_at)
    {
        if (sim->stop)
            return ;
        usleep(100);
    }
}

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

void request_dongle(t_simulation *sim, t_coder *coder, t_dongle *dongle)
{
    pthread_mutex_lock(&dongle->mutex);

    t_request *req = create_request(sim, coder);

    dongle->heap[dongle->size] = req;
    push_heap(dongle, req, sim->scheduler);
    heapify_up(dongle, dongle->size, sim->scheduler);
    printf("coder -> %d, requests dongle", coder->id);
    dongle->size++;
    printf("\n\nreq = %d et dongle->heap[0] = %d\n", req->coder_id, dongle->heap[0]->coder_id);
    printf("req = %ld et dongle->heap[0] = %ld\n", req->arrival_time, dongle->heap[0]->arrival_time);
    printf("req = %ld et dongle->heap[0] = %ld\n\n\n", req->deadline, dongle->heap[0]->deadline);
    if (dongle->heap[0] != req)
        printf("dongle->heap[0] != req");
    if (!dongle_available(dongle))
        printf("!dongle_available(dongle)");
    while (!dongle_available(dongle) || dongle->heap[0] != req)
    {
        printf("\n=============================================================================================CR7\n");
        pthread_cond_wait(&dongle->cond, &dongle->mutex);
        if (sim->stop)
        {
            pthread_mutex_unlock(&dongle->mutex);
            return;
        }
    }
    pop_heap(dongle, sim->scheduler);
    // heapify_down(dongle, index, sim->scheduler);
    dongle->is_taken = 1;

    pthread_mutex_unlock(&dongle->mutex);
}

int get_stop(t_simulation *sim)
{
    int stop;

    pthread_mutex_lock(&sim->stop_mutex);
    stop = sim->stop;
    pthread_mutex_unlock(&sim->stop_mutex);
    return (stop);
}
