/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap_manag_norm.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fchaput <fchaput@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/19 09:39:15 by fchaput           #+#    #+#             */
/*   Updated: 2026/06/19 10:28:59 by fchaput          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	push_heap(t_dongle *d, t_request *req, char *scheduler)
{
	if (d->size >= d->capacity)
		return (0);
	d->heap[d->size] = req;
	heapify_up(d, d->size, scheduler);
	d->size++;
	return (1);
}

void	remove_request(t_dongle *d, t_request *req, char *scheduler)
{
	int	i;

	i = 0;
	while (i < d->size)
	{
		if (d->heap[i] == req)
		{
			d->heap[i] = d->heap[d->size - 1];
			d->size--;
			if (i < d->size)
			{
				heapify_up(d, i, scheduler);
				heapify_down(d, i, scheduler);
			}
			return ;
		}
		i++;
	}
}

void	start_coders(t_simulation *sim)
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
