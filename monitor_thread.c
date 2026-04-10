#include "codexion.h"
void *monitor_thread(void *arg)
{
    t_simulation *sim = (t_simulation *)arg;
    int i;
    long now;
    long last;

    while (1)
    {
        i = 0;
        while (i < sim->number_of_coders)
        {
            pthread_mutex_lock(&sim->coders[i].lock);
            last = sim->coders[i].last_compile;
            pthread_mutex_unlock(&sim->coders[i].lock);

            now = get_time_in_ms();

            pthread_mutex_lock(&sim->stop_mutex);
            if (sim->stop)
            {
                pthread_mutex_unlock(&sim->stop_mutex);
                return NULL;
            }
            pthread_mutex_unlock(&sim->stop_mutex);

            if (now - last > sim->time_to_burnout)
            {
                pthread_mutex_lock(&sim->stop_mutex);
                sim->stop = 1;
                pthread_mutex_unlock(&sim->stop_mutex);

                write(1, "BURNOUT\n", 8);
                return NULL;
            }
            i++;
        }
        usleep(1000);
    }
}
