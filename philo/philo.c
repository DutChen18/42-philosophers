/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philo.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: csteenvo <csteenvo@student.codam.n>          +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/03/31 10:40:02 by csteenvo      #+#    #+#                 */
/*   Updated: 2022/03/31 16:27:30 by csteenvo      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void
	ptake(t_seat *seat, t_seat *fork, int *state, int *other)
{
	if (!*state)
	{
		pthread_mutex_lock(&fork->mutex);
		if (!fork->state)
		{
			pthread_mutex_lock(&seat->info->mutex);
			seat->info->now = ptime();
			if (!seat->info->done)
			{
				*state = 1;
				fork->state = 1;
				pputs(seat, "has taken a fork");
				if (*other)
				{
					seat->t_eat = seat->info->now;
					seat->n_eat += 1;
					pputs(seat, "is eating");
					seat->info->n_fed += (seat->n_eat == seat->info->n_eat);
					seat->info->done = (seat->info->n_fed == seat->info->count);
				}
			}
			pthread_mutex_unlock(&seat->info->mutex);
		}
		pthread_mutex_unlock(&fork->mutex);
	}
}

void
	pdrop(t_seat *fork)
{
	pthread_mutex_lock(&fork->mutex);
	fork->state = 0;
	pthread_mutex_unlock(&fork->mutex);
}

void
	*pstart(void *ptr)
{
	t_seat	*seat;
	t_seat	*next;
	int		state[2];

	seat = ptr;
	next = &seat->info->seats[(seat->index + 1) % seat->info->count];
	while (pcheck(seat, "is thinking"))
	{
		state[0] = 0;
		state[1] = 0;
		while (!state[0] || !state[1])
		{
			if (!pcheck(seat, NULL))
				return (NULL);
			ptake(seat, seat, &state[0], &state[1]);
			ptake(seat, next, &state[1], &state[0]);
		}
		psleep(seat->info->t_eat);
		pdrop(seat);
		pdrop(next);
		if (!pcheck(seat, "is sleeping"))
			return (NULL);
		psleep(seat->info->t_slp);
	}
	return (NULL);
}
