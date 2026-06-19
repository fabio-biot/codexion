/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap_management.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fchaput <fchaput@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/19 09:28:33 by fchaput           #+#    #+#             */
/*   Updated: 2026/06/19 09:39:58 by fchaput          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	compare(t_request *a, t_request *b, char *scheduler)
{
	if (strcmp(scheduler, "fifo") == 0)
		return (a->arrival_time < b->arrival_time);
	else
		return (a->deadline < b->deadline);
}

void	heapify_up(t_dongle *d, int index, char *scheduler)
{
	int	parent;

	while (index > 0)
	{
		parent = (index - 1) / 2;
		if (compare(d->heap[index], d->heap[parent], scheduler))
		{
			swap(&d->heap[index], &d->heap[parent]);
			index = parent;
		}
		else
			break ;
	}
}

int	get_smallest(t_dongle *d, int index, char *scheduler)
{
	int	left;
	int	right;
	int	smallest;

	left = 2 * index + 1;
	right = 2 * index + 2;
	smallest = index;
	if (left < d->size)
	{
		if (compare(d->heap[left], d->heap[smallest], scheduler))
			smallest = left;
	}
	if (right < d->size)
	{
		if (compare(d->heap[right], d->heap[smallest], scheduler))
			smallest = right;
	}
	return (smallest);
}

void	heapify_down(t_dongle *d, int index, char *scheduler)
{
	int	smallest;

	while (1)
	{
		smallest = get_smallest(d, index, scheduler);
		if (smallest == index)
			break ;
		swap(&d->heap[index], &d->heap[smallest]);
		index = smallest;
	}
}

t_request	*pop_heap(t_dongle *d, char *scheduler)
{
	t_request	*top;

	if (d->size == 0)
		return (NULL);
	top = d->heap[0];
	d->heap[0] = d->heap[d->size - 1];
	d->size--;
	heapify_down(d, 0, scheduler);
	return (top);
}
