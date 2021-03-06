/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   util.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: csteenvo <csteenvo@student.codam.n>          +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/03/31 14:06:55 by csteenvo      #+#    #+#                 */
/*   Updated: 2022/04/15 11:59:15 by csteenvo      ########   odam.nl         */
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
	const long	time = (seat->info->now - seat->info->start) / 1000;

	printf("%ld %d %s\n", time, seat->index + 1, str);
}

int
	pcheck(t_info *info, t_seat *seat, const char *str)
{
	int	done;

	pthread_mutex_lock(&info->mutex);
	done = info->done;
	if (!done && str != NULL)
	{
		info->now = ptime();
		pputs(seat, str);
	}
	pthread_mutex_unlock(&info->mutex);
	return (done);
}

void
	psleep(t_info *info, long delta)
{
	long	end;

	end = ptime() + delta;
	while (delta > 100000 / 0.8)
	{
		usleep(100000);
		if (pcheck(info, NULL, NULL))
			return ;
		delta = end - ptime();
	}
	while (delta > 1000)
	{
		usleep(delta * 0.8);
		delta = end - ptime();
	}
	if (delta > 0)
		usleep(delta);
}

long
	patol(const char *str, long max)
{
	long	v;

	if (!(*str >= '0' || *str <= '9'))
		return (-1);
	v = 0;
	while (*str >= '0' && *str <= '9')
	{
		if (v > max / 10 || v * 10 > max - (*str - '0'))
			return (-1);
		v = v * 10 + (*str - '0');
		str += 1;
	}
	if (*str != '\0')
		return (-1);
	return (v);
}
