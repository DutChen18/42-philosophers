/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   util.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: csteenvo <csteenvo@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/11 13:31:49 by csteenvo      #+#    #+#                 */
/*   Updated: 2022/02/17 13:21:46 by csteenvo      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <sys/time.h>
#include <unistd.h>
#include <limits.h>

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

int
	patoi(const char *str)
{
	long	result;

	result = 0;
	while (*str != '\0')
	{
		if (*str < '0' || *str > '9')
			return (-1);
		result = result * 10 + *str - '0';
		if (result > INT_MAX)
			return (-1);
		str += 1;
	}
	return (result);
}
