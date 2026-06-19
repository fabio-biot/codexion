/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_init.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fchaput <fchaput@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/19 10:30:50 by fchaput           #+#    #+#             */
/*   Updated: 2026/06/19 11:51:22 by fchaput          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	is_valid_argument(int i, char *arg)
{
	if (!is_number(arg) && i > 0 && i < 8)
	{
		printf("Argument %d should be an int\n", i);
		return (0);
	}
	return (1);
}

int	is_valid_scheduler(int i, char *arg)
{
	if (i == 8)
	{
		if (strcmp(arg, "fifo") != 0 && strcmp(arg, "edf") != 0)
		{
			printf("Argument %d should be 'fifo' or 'edf'\n", i);
			return (0);
		}
	}
	return (1);
}

t_simulation	*init_sim(int *args, char *scheduler)
{
	t_simulation	*sim;

	sim = malloc(sizeof(t_simulation));
	if (!sim)
		return (NULL);
	sim->number_of_coders = args[0];
	sim->time_to_burnout = args[1];
	sim->time_to_compile = args[2];
	sim->time_to_debug = args[3];
	sim->time_to_refactor = args[4];
	sim->number_of_compiles_required = args[5];
	sim->dongle_cooldown = args[6];
	sim->scheduler = scheduler;
	sim->stop = 0;
	sim->start_time = get_time_in_ms();
	pthread_mutex_init(&sim->print_mutex, NULL);
	pthread_mutex_init(&sim->stop_mutex, NULL);
	sim->dongles = init_dongles(sim);
	sim->coders = init_coders(sim);
	return (sim);
}

t_coder	*init_coders(t_simulation *sim)
{
	t_coder	*coders;
	int		i;

	coders = malloc(sizeof(t_coder) * sim->number_of_coders);
	if (!coders)
		return (NULL);
	i = 0;
	while (i < sim->number_of_coders)
	{
		coders[i].id = i + 1;
		coders[i].compile_count = 0;
		coders[i].last_compile_start = sim->start_time;
		coders[i].sim = sim;
		coders[i].left = &sim->dongles[i];
		coders[i].right = &sim->dongles[(i + 1) % sim->number_of_coders];
		coders[i].thread = 0;
		pthread_mutex_init(&coders[i].lock, NULL);
		i++;
	}
	return (coders);
}

t_dongle	*init_dongles(t_simulation *sim)
{
	t_dongle	*dongles;
	int			i;

	i = 0;
	dongles = malloc(sizeof(t_dongle) * sim->number_of_coders);
	while (i < sim->number_of_coders)
	{
		pthread_mutex_init(&dongles[i].mutex, NULL);
		pthread_cond_init(&dongles[i].cond, NULL);
		dongles[i].heap = malloc(sizeof(t_request *) * sim->number_of_coders);
		dongles[i].size = 0;
		dongles[i].capacity = sim->number_of_coders;
		dongles[i].is_taken = 0;
		dongles[i].available_at = 0;
		dongles[i].sim = sim;
		i++;
	}
	return (dongles);
}

// int main(int argc, char *argv[])
// {
//	 int *args;
//	 t_simulation *sim;

//	 args = parsing_args(argc, argv);
//	 if (!args)
//		 return (1);

//	 sim = init_sim(argc, args);
//	 if (!sim)
//		 return (1);

//	 printf("Coders: %d\n", sim->number_of_coders);
//	 printf("Burnout: %ld\n", sim->time_to_burnout);

//	 return 0;
// }