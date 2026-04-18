#include "codexion.h"

void *process_take_dongle(t_coder *coder, t_simulation *sim, t_dongle *dongle)
{
    if (sim->stop)
        return NULL;
    request_dongle(sim, coder, dongle);
    print_state(sim, coder,"has taken a dongle", sim->start_time);
    return(0);
}
void *compile_cooldown_debug_refactor(t_coder *coder, t_simulation *sim)
{
    long now;

    printf("\n\nCOMPILE_COOLDOWN\n\n");
    pthread_mutex_lock(&coder->lock);
    coder->last_compile_start = get_time_in_ms();
    pthread_mutex_unlock(&coder->lock);
    print_state(sim, coder, "is compiling", sim->start_time);
    usleep(sim->time_to_compile * 1000);
    now = get_time_in_ms();

    coder->right->available_at = now + sim->dongle_cooldown;
    coder->left->available_at = now + sim->dongle_cooldown;
    coder->right->is_taken = 0;
    coder->left->is_taken = 0;
    pthread_cond_broadcast(&coder->left->cond);
    pthread_cond_broadcast(&coder->right->cond);
    pthread_mutex_unlock(&coder->right->mutex);
    pthread_mutex_unlock(&coder->left->mutex);

    print_state(sim, coder, "is debugging", sim->start_time);
    usleep(sim->time_to_debug * 1000);

    print_state(sim, coder, "is refactoring", sim->start_time);
    usleep(sim->time_to_refactor * 1000);
    return(0);
}
