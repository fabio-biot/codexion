/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_thread.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fchaput <fchaput@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/19 10:29:46 by fchaput           #+#    #+#             */
/*   Updated: 2026/06/19 10:29:51 by fchaput          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	check_burnout(t_simulation *sim, int i, long now)
{
	long	last;

	pthread_mutex_lock(&sim->coders[i].lock);
	last = sim->coders[i].last_compile_start;
	pthread_mutex_unlock(&sim->coders[i].lock);
	if (now - last > sim->time_to_burnout)
		return (1);
	return (0);
}

static void	handle_burnout(t_simulation *sim, int i)
{
	int	j;

	pthread_mutex_lock(&sim->stop_mutex);
	sim->stop = 1;
	pthread_mutex_unlock(&sim->stop_mutex);
	pthread_mutex_lock(&sim->print_mutex);
	printf("%ld %d burned out\n",
		get_time_in_ms() - sim->start_time, i + 1);
	pthread_mutex_unlock(&sim->print_mutex);
	j = 0;
	while (j < sim->number_of_coders)
	{
		pthread_mutex_lock(&sim->dongles[j].mutex);
		pthread_cond_broadcast(&sim->dongles[j].cond);
		pthread_mutex_unlock(&sim->dongles[j].mutex);
		j++;
	}
}

static int	check_finished(t_simulation *sim)
{
	pthread_mutex_lock(&sim->stop_mutex);
	if (sim->number_of_coders == sim->coders_ended)
	{
		sim->stop = 1;
		pthread_mutex_unlock(&sim->stop_mutex);
		return (1);
	}
	pthread_mutex_unlock(&sim->stop_mutex);
	return (0);
}

void	*monitor_thread(void *arg)
{
	t_simulation	*sim;
	int				i;
	long			now;

	sim = (t_simulation *)arg;
	while (1)
	{
		i = 0;
		while (i < sim->number_of_coders)
		{
			now = get_time_in_ms();
			if (check_burnout(sim, i, now))
			{
				handle_burnout(sim, i);
				return (NULL);
			}
			if (check_finished(sim))
				return (NULL);
			i++;
		}
		usleep(1000);
	}
}
