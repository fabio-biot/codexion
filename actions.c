#include "codexion.h"

void *process_take_dongle(t_coder *coder, t_simulation *sim, t_dongle *dongle)
{
    if (get_stop(sim))
        return NULL;
    request_dongle(sim, coder, dongle);
    print_state(sim, coder,"has taken a dongle", sim->start_time);
    return(0);
}
void release_dongle(t_dongle *d)
{
    pthread_mutex_lock(&d->mutex);
    d->is_taken = 0;
    d->available_at = get_time_in_ms();
    pthread_cond_broadcast(&d->cond);
    pthread_mutex_unlock(&d->mutex);
}