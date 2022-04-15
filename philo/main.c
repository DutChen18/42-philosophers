/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: csteenvo <csteenvo@student.codam.n>          +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/03/31 15:53:37 by csteenvo      #+#    #+#                 */
/*   Updated: 2022/04/15 11:06:14 by csteenvo      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

int
	pinit(t_info *info)
{
	int	i;

	info->seats = malloc(sizeof(*info->seats) * info->count);
	if (info->seats != NULL && !pthread_mutex_init(&info->mutex, NULL))
	{
		i = 0;
		while (i < info->count)
		{
			info->seats[i].info = info;
			info->seats[i].index = i;
			info->seats[i].n_eat = 0;
			info->seats[i].state = 0;
			if (pthread_mutex_init(&info->seats[i].mutex, NULL))
				break ;
			i += 1;
		}
		if (i == info->count)
			return (0);
		while (i > 0)
			pthread_mutex_destroy(&info->seats[--i].mutex);
		pthread_mutex_destroy(&info->mutex);
	}
	free(info->seats);
	return (1);
}

void
	ploop(t_info *info)
{
	int	done;
	int	i;

	done = 0;
	while (!done)
	{
		pthread_mutex_lock(&info->mutex);
		info->now = ptime();
		i = 0;
		while (!info->done && i < info->count)
		{
			if (info->seats[i].t_eat + info->t_die <= info->now)
			{
				pputs(&info->seats[i], "died");
				info->done = 1;
			}
			i += 1;
		}
		done = info->done;
		pthread_mutex_unlock(&info->mutex);
		if (!done)
			psleep(info, 1000);
	}
}

void
	pstop(t_info *info, int i)
{
	pthread_mutex_lock(&info->mutex);
	info->done = 1;
	pthread_mutex_unlock(&info->mutex);
	while (i > 0)
		pthread_join(info->seats[--i].thread, NULL);
	pthread_mutex_destroy(&info->mutex);
	while (i < info->count)
		pthread_mutex_destroy(&info->seats[i++].mutex);
	free(info->seats);
}

void
	prun(t_info *info)
{
	int	i;

	if (!pinit(info))
	{
		info->start = ptime();
		i = 0;
		while (i < info->count)
		{
			info->seats[i].t_eat = info->start;
			if (pthread_create(&info->seats[i].thread,
					NULL, pstart, &info->seats[i]))
				break ;
			i += 1;
		}
		if (i == info->count)
			ploop(info);
		pstop(info, i);
	}
}

int
	main(int argc, char **argv)
{
	t_info	info;

	if (argc < 5 || argc > 6)
		return (EXIT_FAILURE);
	info.done = 0;
	info.n_eat = 0;
	info.n_fed = 0;
	info.count = patol(argv[1], INT_MAX);
	info.t_die = patol(argv[2], LONG_MAX / 1000) * 1000;
	info.t_eat = patol(argv[3], LONG_MAX / 1000) * 1000;
	info.t_slp = patol(argv[4], LONG_MAX / 1000) * 1000;
	if (argc == 6)
		info.n_eat = patol(argv[5], INT_MAX);
	if (info.n_eat < 0 || info.count < 1 || info.t_die < 1
		|| info.t_eat < 1 || info.t_slp < 1)
		return (EXIT_FAILURE);
	prun(&info);
	return (EXIT_SUCCESS);
}
