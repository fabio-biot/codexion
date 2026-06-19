/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fchaput <fchaput@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/19 09:50:13 by fchaput           #+#    #+#             */
/*   Updated: 2026/06/19 10:28:11 by fchaput          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	take_dongles(t_coder *coder, t_simulation *sim,
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

int	coder_cycle(t_coder *coder, t_simulation *sim)
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

void	post_cycle(t_simulation *sim, t_coder *coder)
{
	print_state(sim, coder, "is debugging", sim->start_time);
	usleep(sim->time_to_debug * 1000);
	print_state(sim, coder, "is refactoring", sim->start_time);
	usleep(sim->time_to_refactor * 1000);
}

static int	run_cycle(t_coder *coder, t_simulation *sim, int *i)
{
	if (sim->number_of_coders == 1)
	{
		print_state(sim, coder, "has taken a dongle",
			sim->start_time);
		while (!get_stop(sim))
			usleep(1000);
		return (1);
	}
	if (coder_cycle(coder, sim))
		return (1);
	(*i)++;
	return (0);
}

void	*coder_routine(void *arg)
{
	t_coder			*coder;
	t_simulation	*sim;
	int				i;

	coder = (t_coder *)arg;
	sim = coder->sim;
	i = 0;
	while (!get_stop(sim)
		&& i < sim->number_of_compiles_required)
	{
		if (run_cycle(coder, sim, &i))
			break ;
		post_cycle(sim, coder);
	}
	pthread_mutex_lock(&sim->stop_mutex);
	sim->coders_ended++;
	pthread_mutex_unlock(&sim->stop_mutex);
	return (NULL);
}
