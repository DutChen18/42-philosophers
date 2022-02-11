/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   util.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: csteenvo <csteenvo@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/11 13:31:49 by csteenvo      #+#    #+#                 */
/*   Updated: 2022/02/11 14:59:00 by csteenvo      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>

long
	ptime(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((long) tv.tv_sec * 1000000 + tv.tv_usec);
}

int
	plock(t_info *info)
{
	pthread_mutex_lock(&info->mutex);
	if (info->done)
	{
		pthread_mutex_unlock(&info->mutex);
		return (0);
	}
	return (1);
}

long
	pwait(t_info *info, long end)
{
	long	time;

	while (1)
	{
		if (!plock(info))
			return (-1);
		time = ptime();
		if (time >= end)
			break ;
		pthread_mutex_unlock(&info->mutex);
		usleep(1000);
	}
	return (time);
}

long
	psleep(t_info *info, long delta)
{
	long	time;

	time = pwait(info, ptime() + delta);
	if (time >= 0)
		pthread_mutex_unlock(&info->mutex);
	return (time);
}

void
	plog(t_seat *seat, long time, const char *str)
{
	time = (time - seat->info->start) / 1000;
	printf("%ld %d %s\n", time, seat->index + 1, str);
	fflush(stdout);
}
