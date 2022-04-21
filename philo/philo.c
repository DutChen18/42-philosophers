/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philo.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: csteenvo <csteenvo@student.codam.n>          +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/03/31 10:40:02 by csteenvo      #+#    #+#                 */
/*   Updated: 2022/04/21 10:55:06 by csteenvo      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void
	ptake(t_seat *seat, t_seat *fork, int *state, int *other)
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

void
	ptake1(t_seat *seat, t_seat *fork, int *state, int *other)
{
	if (!*state)
	{
		pthread_mutex_lock(&fork->mutex);
		if (!fork->state)
		{
			pthread_mutex_lock(&seat->info->mutex);
			seat->info->now = ptime();
			if (seat->t_eat + seat->info->t_die > seat->info->now
				&& !seat->info->done)
				ptake(seat, fork, state, other);
			pthread_mutex_unlock(&seat->info->mutex);
		}
		pthread_mutex_unlock(&fork->mutex);
	}
}

int
	ptake2(t_seat *seat, t_seat *next)
{
	int	state[2];

	state[0] = 0;
	state[1] = 0;
	while (!state[0] || !state[1])
	{
		if (pcheck(seat->info, seat, NULL))
			return (1);
		ptake1(seat, seat, &state[0], &state[1]);
		ptake1(seat, next, &state[1], &state[0]);
		if (!state[0] || !state[1])
			psleep(seat->info, 1000);
	}
	return (0);
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

	seat = ptr;
	next = &seat->info->seats[(seat->index + 1) % seat->info->count];
	if (seat->index % 2 == 1)
		psleep(seat->info, 10000);
	while (!pcheck(seat->info, seat, "is thinking"))
	{
		if (ptake2(seat, next))
			return (NULL);
		psleep(seat->info, seat->info->t_eat);
		pdrop(seat);
		pdrop(next);
		if (pcheck(seat->info, seat, "is sleeping"))
			return (NULL);
		psleep(seat->info, seat->info->t_slp);
	}
	return (NULL);
}
