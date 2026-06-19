/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fchaput <fchaput@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/19 09:40:53 by fchaput           #+#    #+#             */
/*   Updated: 2026/06/19 09:55:00 by fchaput          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long	get_time_in_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void	join_all(t_simulation *sim)
{
	int	i;

	i = 0;
	while (i < sim->number_of_coders)
	{
		pthread_join(sim->coders[i].thread, NULL);
		i++;
	}
	pthread_join(sim->thread_monitor, NULL);
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
