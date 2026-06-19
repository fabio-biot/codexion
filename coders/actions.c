/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fchaput <fchaput@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/19 09:13:42 by fchaput           #+#    #+#             */
/*   Updated: 2026/06/19 09:14:53 by fchaput          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	*process_take_dongle(t_coder *coder, t_simulation *sim,
	t_dongle *dongle)
{
	if (get_stop(sim))
		return (NULL);
	request_dongle(sim, coder, dongle);
	print_state(sim, coder, "has taken a dongle", sim->start_time);
	return (0);
}

void	release_dongle(t_dongle *d)
{
	pthread_mutex_lock(&d->mutex);
	d->is_taken = 0;
	d->available_at = get_time_in_ms() + d->sim->dongle_cooldown;
	pthread_cond_broadcast(&d->cond);
	pthread_mutex_unlock(&d->mutex);
}
