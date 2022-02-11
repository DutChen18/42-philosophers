/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philo.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: csteenvo <csteenvo@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/11 11:03:24 by csteenvo      #+#    #+#                 */
/*   Updated: 2022/02/11 14:54:10 by csteenvo      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int
	ptake1(t_seat *seat, t_seat *fork, int *forks)
{
	long	time;

	if (fork->flag)
		return (1);
	fork->flag = 1;
	if (!plock(seat->info))
		return (0);
	time = ptime();
	plog(seat, time, "has taken a fork");
	*forks += 1;
	if (*forks == 2)
	{
		plog(seat, time, "is eating");
		seat->t_eat = time;
		seat->n_eat += 1;
		seat->info->n_fed += seat->info->n_eat == seat->n_eat;
		if (seat->info->n_fed == seat->info->count)
		{
			seat->info->done = 1;
			pthread_mutex_unlock(&seat->info->mutex);
			return (0);
		}
	}
	pthread_mutex_unlock(&seat->info->mutex);
	return (1);
}

int
	ptake2(t_info *info, t_seat *seat)
{
	t_seat	*fork1;
	t_seat	*fork2;
	int		forks;

	fork1 = &info->seats[(seat->index + ((seat->index & 1) ^ 0)) % info->count];
	fork2 = &info->seats[(seat->index + ((seat->index & 1) ^ 1)) % info->count];
	forks = 0;
	while (1)
	{
		pthread_mutex_lock(&fork1->mutex);
		pthread_mutex_lock(&fork2->mutex);
		if (!ptake1(seat, fork1, &forks) || !ptake1(seat, fork2, &forks))
		{
			pthread_mutex_unlock(&fork2->mutex);
			pthread_mutex_unlock(&fork1->mutex);
			return (0);
		}
		pthread_mutex_unlock(&fork2->mutex);
		pthread_mutex_unlock(&fork1->mutex);
		if (forks == 2)
			return (1);
		if (psleep(seat->info, 1000) < 0)
			return (0);
	}
}

void
	pdrop2(t_info *info, t_seat *seat)
{
	t_seat	*fork1;
	t_seat	*fork2;

	fork1 = &info->seats[(seat->index + 0) % info->count];
	pthread_mutex_lock(&fork1->mutex);
	fork1->flag = 0;
	pthread_mutex_unlock(&fork1->mutex);
	fork2 = &info->seats[(seat->index + 1) % info->count];
	pthread_mutex_lock(&fork2->mutex);
	fork2->flag = 0;
	pthread_mutex_unlock(&fork2->mutex);
}

void
	*pstart(void *ptr)
{
	long	time;
	t_seat	*seat;

	seat = ptr;
	if (!plock(seat->info))
		return (NULL);
	time = ptime();
	while (1)
	{
		plog(seat, time, "is thinking");
		pthread_mutex_unlock(&seat->info->mutex);
		if (!ptake2(seat->info, seat))
			return (NULL);
		time = pwait(seat->info, seat->t_eat + seat->info->t_eat);
		if (time < 0)
			return (NULL);
		plog(seat, time, "is sleeping");
		pthread_mutex_unlock(&seat->info->mutex);
		pdrop2(seat->info, seat);
		time = pwait(seat->info, time + seat->info->t_slp);
		if (time < 0)
			return (NULL);
	}
}
