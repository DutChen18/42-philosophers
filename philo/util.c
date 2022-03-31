/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   util.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: csteenvo <csteenvo@student.codam.n>          +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/03/31 14:06:55 by csteenvo      #+#    #+#                 */
/*   Updated: 2022/03/31 16:28:08 by csteenvo      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>

long
	ptime(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((long) tv.tv_sec * 1000000 + tv.tv_usec);
}

void
	pputs(t_seat *seat, const char *str)
{
	const long	time = (seat->info->now - seat->info->start) / 1000000;

	printf("%ld %d %s\n", time, seat->index + 1, str);
}

int
	pcheck(t_seat *seat, const char *str)
{
	int	done;

	pthread_mutex_lock(&seat->info->mutex);
	done = seat->info->done;
	if (!done && str != NULL)
	{
		seat->info->now = ptime();
		pputs(seat, str);
	}
	pthread_mutex_unlock(&seat->info->mutex);
	return (!done);
}

void
	psleep(long delta)
{
	usleep(delta);
}
