void *coder_routine(void *arg)
{
    t_coder *coder = (t_coder *)arg;
    t_simulation *sim = coder->sim;
    int i;
    long now;
 
    i = 0;
    while (sim->number_of_compiles_required > i)
    {
        pthread_mutex_lock(&sim->stop_mutex);
        if (sim->stop)
        {
            pthread_mutex_unlock(&sim->stop_mutex);
            return NULL;
        }
        pthread_mutex_unlock(&sim->stop_mutex);
        if (coder->id % 2 == 0)
        {
            wait(coder->right->available_at, sim);
            pthread_mutex_lock(&coder->right->mutex);
            print_state(sim, coder, "has taken a dongle", sim->start_time);
            
            pthread_mutex_lock(&coder->left->mutex);
            wait(coder->left->available_at, sim);
            print_state(sim, coder, "has taken a dongle", sim->start_time);
        }
        else
        {
            pthread_mutex_lock(&coder->left->mutex);
            wait(coder->right->available_at, sim);
            print_state(sim, coder, "has taken a dongle", sim->start_time);

            pthread_mutex_lock(&coder->right->mutex);
            wait(coder->right->available_at, sim);
            print_state(sim, coder, "has taken a dongle", sim->start_time);
        }

        pthread_mutex_lock(&coder->lock);
        coder->last_compile_start = get_time_in_ms();
        pthread_mutex_unlock(&coder->lock);
        print_state(sim, coder, "is compiling", sim->start_time);
        usleep(sim->time_to_compile * 1000);
        now = get_time_in_ms();

        coder->right->available_at = now + sim->dongle_cooldown;
        coder->left->available_at = now + sim->dongle_cooldown;
        pthread_mutex_unlock(&coder->right->mutex);
        pthread_mutex_unlock(&coder->left->mutex);

        print_state(sim, coder, "is debugging", sim->start_time);
        usleep(sim->time_to_debug * 1000);

        print_state(sim, coder, "is refactoring", sim->start_time);
        usleep(sim->time_to_refactor * 1000);

        i++;
        coder->compile_count++;
    }
    return NULL;
}