/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   util.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: csteenvo <csteenvo@student.codam.n>          +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/03/31 14:06:55 by csteenvo      #+#    #+#                 */
/*   Updated: 2022/04/15 12:06:13 by csteenvo      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <errno.h>
#include <stdlib.h>
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
	pputs(t_info *info, const char *str)
{
	const long	time = (info->now - info->start) / 1000;

	printf("%ld %d %s\n", time, info->index + 1, str);
}

void
	pcheck(t_info *info, const char *str)
{
	while (sem_wait(info->mutex))
		if (errno != EINTR)
			exit(EXIT_FAILURE);
	info->now = ptime();
	pputs(info, str);
	if (sem_post(info->mutex))
		exit(EXIT_FAILURE);
}

void
	psleep(long delta)
{
	long	end;

	end = ptime() + delta;
	while (delta > 100000 / 0.8)
	{
		usleep(100000);
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
