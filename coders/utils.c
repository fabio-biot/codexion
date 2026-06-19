/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fchaput <fchaput@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/19 11:57:44 by fchaput           #+#    #+#             */
/*   Updated: 2026/06/19 11:57:45 by fchaput          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	is_number(char *str)
{
	int	i;

	i = 0;
	if (!str || str[0] == '\0')
		return (0);
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}
// void wait(long coder_availiable_at, t_simulation *sim)
// {
//	 while (get_time_in_ms() < coder_availiable_at)
//	 {
//		 if (get_stop(sim))
//			 return ;
//		 usleep(100);
//	 }
// }

int	dongle_available(t_dongle *d)
{
	long	now;

	now = get_time_in_ms();
	if (d->is_taken)
		return (0);
	if (now < d->available_at)
		return (0);
	return (1);
}

t_request	*create_request(t_simulation *sim, t_coder *coder)
{
	t_request	*req;

	req = malloc(sizeof(t_request));
	if (!req)
		return (NULL);
	req->coder_id = coder->id;
	req->arrival_time = get_time_in_ms();
	pthread_mutex_lock(&coder->lock);
	req->deadline = coder->last_compile_start + sim->time_to_burnout;
	pthread_mutex_unlock(&coder->lock);
	return (req);
}

t_request	*init_request(t_simulation *sim,
			t_coder *coder, t_dongle *d)
{
	t_request	*req;

	pthread_mutex_lock(&d->mutex);
	if (get_stop(sim))
	{
		pthread_mutex_unlock(&d->mutex);
		return (NULL);
	}
	req = create_request(sim, coder);
	if (!req)
	{
		pthread_mutex_unlock(&d->mutex);
		return (NULL);
	}
	push_heap(d, req, sim->scheduler);
	return (req);
}

int	can_take_dongle(t_simulation *sim,
			t_dongle *d, t_request *req)
{
	long	now;

	now = get_time_in_ms();
	if (d->heap[0] == req && !d->is_taken && now >= d->available_at)
	{
		pop_heap(d, sim->scheduler);
		d->is_taken = 1;
		return (1);
	}
	return (0);
}
