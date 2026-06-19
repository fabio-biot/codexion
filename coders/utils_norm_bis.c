/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_norm_bis.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fchaput <fchaput@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/19 11:55:51 by fchaput           #+#    #+#             */
/*   Updated: 2026/06/19 11:56:02 by fchaput          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	wait_dongle(t_simulation *sim,
			t_dongle *d, t_request *req, long now)
{
	struct timespec	ts;
	struct timeval	tv;
	long			wait_ms;

	(void)sim;
	if (d->heap[0] == req && !d->is_taken && now < d->available_at)
	{
		gettimeofday(&tv, NULL);
		wait_ms = d->available_at - now;
		ts.tv_sec = tv.tv_sec + (tv.tv_usec / 1000 + wait_ms) / 1000;
		ts.tv_nsec = ((tv.tv_usec / 1000 + wait_ms) % 1000) * 1000000;
		pthread_cond_timedwait(&d->cond, &d->mutex, &ts);
	}
	else
		pthread_cond_wait(&d->cond, &d->mutex);
}

void	request_dongle(t_simulation *sim,
			t_coder *coder, t_dongle *d)
{
	t_request	*req;
	long		now;

	req = init_request(sim, coder, d);
	if (!req)
	{
		pthread_mutex_unlock(&d->mutex);
		return ;
	}
	while (1)
	{
		if (get_stop(sim))
		{
			remove_request(d, req, sim->scheduler);
			free(req);
			pthread_mutex_unlock(&d->mutex);
			return ;
		}
		now = get_time_in_ms();
		if (can_take_dongle(sim, d, req))
		{
			free(req);
			pthread_mutex_unlock(&d->mutex);
			return ;
		}
		wait_dongle(sim, d, req, now);
	}
}
