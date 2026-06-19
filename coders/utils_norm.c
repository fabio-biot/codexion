/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_norm.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fchaput <fchaput@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/19 11:54:19 by fchaput           #+#    #+#             */
/*   Updated: 2026/06/19 11:58:15 by fchaput          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	get_stop(t_simulation *sim)
{
	int	stop;

	pthread_mutex_lock(&sim->stop_mutex);
	stop = sim->stop;
	pthread_mutex_unlock(&sim->stop_mutex);
	return (stop);
}

void	swap(t_request **a, t_request **b)
{
	t_request	*tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

void	print_state(t_simulation *sim, t_coder *coder, char *msg, long start)
{
	pthread_mutex_lock(&sim->print_mutex);
	printf("%ld %d %s\n", get_time_in_ms() - start, coder->id, msg);
	pthread_mutex_unlock(&sim->print_mutex);
}

int	is_positive(int i, char *arg)
{
	int	min;

	min = 1;
	if (i == 6)
		min = 0;
	if (atoi(arg) < min)
	{
		printf("Argument %d should be positive\n", i);
		return (0);
	}
	return (1);
}
