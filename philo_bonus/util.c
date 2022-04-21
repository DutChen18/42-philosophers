/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   util.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: csteenvo <csteenvo@student.codam.n>          +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/03/31 14:06:55 by csteenvo      #+#    #+#                 */
/*   Updated: 2022/04/21 14:28:03 by csteenvo      ########   odam.nl         */
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
	psem(sem_t *sem, int state)
{
	if (state)
	{
		while (sem_wait(sem))
			if (errno != EINTR)
				exit(-1);
	}
	else
	{
		if (sem_post(sem))
			exit(-1);
	}
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
