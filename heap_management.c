#include "codexion.h"

int compare(t_request *a, t_request *b, char *scheduler)
{
    if (strcmp(scheduler, "fifo") == 0)
        return (a->arrival_time < b->arrival_time);
    else
        return (a->deadline < b->deadline);
}

void swap(t_request **a, t_request **b)
{
    t_request *tmp;

    tmp = *a;
    *a = *b;
    *b = tmp;
}

void heapify_up(t_dongle *d, int index, char *scheduler)
{
    int parent;

    while (index > 0)
    {
        parent = (index - 1) / 2;
        if (compare(d->heap[index], d->heap[parent], scheduler))
        {
            swap(&d->heap[index], &d->heap[parent]);
            index = parent;
        }
        else
            break;
    }
}

void heapify_down(t_dongle *d, int index, char *scheduler)
{
    int left;
    int right;
    int smallest;

    while (1)
    {
        left = 2 * index + 1;
        right = 2 * index + 2;
        smallest = index;

        if (left < d->size &&
            compare(d->heap[left], d->heap[smallest], scheduler))
            smallest = left;

        if (right < d->size &&
            compare(d->heap[right], d->heap[smallest], scheduler))
            smallest = right;

        if (smallest != index)
        {
            swap(&d->heap[index], &d->heap[smallest]);
            index = smallest;
        }
        else
            break;
    }
}

t_request *pop_heap(t_dongle *d, char *scheduler)
{
    t_request *top;

    if (d->size == 0)
        return (NULL);
    top = d->heap[0];
    d->heap[0] = d->heap[d->size - 1];
    d->size--;
    heapify_down(d, 0, scheduler);
    return (top);
}

int push_heap(t_dongle *d, t_request *req, char *scheduler)
{
    if (d->size >= d->capacity)
        return (0);

    d->heap[d->size] = req;
    heapify_up(d, d->size, scheduler);
    d->size++;
    return (1);
}
