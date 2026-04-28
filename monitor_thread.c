#include "codexion.h"
void *monitor_thread(void *arg)
{
    t_simulation *sim = (t_simulation *)arg;
    int i;
    int j;
    long now;
    long last;

    while (1)
    {
        i = 0;
        while (i < sim->number_of_coders)
        {
            pthread_mutex_lock(&sim->coders[i].lock);
            last = sim->coders[i].last_compile_start;
            pthread_mutex_unlock(&sim->coders[i].lock);
            now = get_time_in_ms();
            if (now - last > sim->time_to_burnout)
            {
                pthread_mutex_lock(&sim->stop_mutex);
                sim->stop = 1;
                pthread_mutex_unlock(&sim->stop_mutex);
                printf("%ld %d burned out\n",  get_time_in_ms() - sim->start_time, i + 1);
                j = 0;
                while(j < sim->number_of_coders)
                {
                    pthread_mutex_lock(&sim->dongles[j].mutex);
                    pthread_cond_broadcast(&sim->dongles[j].cond);
                    pthread_mutex_unlock(&sim->dongles[j].mutex);
                    j++;
                }
                return NULL;
            }
            if (sim->number_of_coders == sim->coders_ended)
            {
                pthread_mutex_lock(&sim->stop_mutex);
                sim->stop = 1;
                pthread_mutex_unlock(&sim->stop_mutex);
                return NULL;
            }
            i++;
        }
        usleep(1000);
    }
}
