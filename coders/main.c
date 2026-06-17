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

static int	take_dongles(t_coder *coder, t_simulation *sim,
    t_dongle **first, t_dongle **second)
{
    if (coder->left < coder->right)
    {
        *first = coder->left;
        *second = coder->right;
    }
    else
    {
        *first = coder->right;
        *second = coder->left;
    }
    process_take_dongle(coder, sim, *first);
    if (get_stop(sim))
        return (1);
    process_take_dongle(coder, sim, *second);
    if (get_stop(sim))
        return (1);
    return (0);
}

static int	coder_cycle(t_coder *coder, t_simulation *sim)
{
	t_dongle	*first;
	t_dongle	*second;

	if (take_dongles(coder, sim, &first, &second))
		return (1);
	pthread_mutex_lock(&coder->lock);
	coder->last_compile_start = get_time_in_ms();
	pthread_mutex_unlock(&coder->lock);
	print_state(sim, coder, "is compiling", sim->start_time);
	usleep(sim->time_to_compile * 1000);
	if (get_stop(sim))
		return (1);
	release_dongle(first);
	release_dongle(second);
	coder->compile_count++;
	return (0);
}

void	*coder_routine(void *arg)
{
	t_coder		*coder;
	t_simulation	*sim;
	int			i;

	coder = (t_coder *)arg;
	sim = coder->sim;
	i = 0;
	while (!get_stop(sim)
		&& i < sim->number_of_compiles_required)
	{
		if (sim->number_of_coders == 1)
		{
			print_state(sim, coder, "has taken a dongle",
				sim->start_time);
			while (!get_stop(sim))
				usleep(1000);
			break ;
		}
		if (coder_cycle(coder, sim))
			break ;
		i++;
		print_state(sim, coder, "is debugging", sim->start_time);
		usleep(sim->time_to_debug * 1000);
		print_state(sim, coder, "is refactoring", sim->start_time);
		usleep(sim->time_to_refactor * 1000);
	}
	pthread_mutex_lock(&sim->stop_mutex);
	sim->coders_ended++;
	pthread_mutex_unlock(&sim->stop_mutex);
	return (NULL);
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

static void	start_coders(t_simulation *sim)
{
	int	i;

	i = 0;
	while (i < sim->number_of_coders)
	{
		pthread_create(&sim->coders[i].thread, NULL,
			coder_routine, &sim->coders[i]);
		i++;
	}
}

static void	start_monitor(t_simulation *sim)
{
	pthread_create(&sim->thread_monitor, NULL,
		monitor_thread, sim);
}

void	cleanup_simulation(t_simulation *sim)
{
	int	i;

	if (!sim)
		return ;
	i = 0;
	while (i < sim->number_of_coders)
	{
		pthread_mutex_destroy(&sim->dongles[i].mutex);
		i++;
	}
	i = 0;
	while (i < sim->number_of_coders)
	{
		pthread_mutex_destroy(&sim->dongles[i].mutex);
		i++;
	}
	pthread_mutex_destroy(&sim->stop_mutex);
	free(sim->coders);
	free(sim->dongles);
	free(sim);
}

int	main(int argc, char *argv[])
{
	int				*args;
	t_simulation	*sim;

	if (argc != 9)
	{
		printf("Invalid args\n");
		return (1);
	}
	args = parsing_args(argc, argv);
	if (!args)
	{
		printf("Failed to parse args\n");
		return (1);
	}
	sim = init_sim(args, argv[8]);
	if (!sim)
	{
		free(args);
		return (1);
	}
	sim->start_time = get_time_in_ms();
	sim->stop = 0;
	start_coders(sim);
	start_monitor(sim);
	join_all(sim);
	cleanup_simulation(sim);
	free(args);
	return (0);
}