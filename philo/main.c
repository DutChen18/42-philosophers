/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: csteenvo <csteenvo@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/11 14:33:50 by csteenvo      #+#    #+#                 */
/*   Updated: 2022/02/17 14:16:47 by csteenvo      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdlib.h>
#include <stdio.h>

static int
	pinit(t_info *info)
{
	int	i;

	info->seats = malloc(sizeof(*info->seats) * info->count);
	if (info->seats == NULL)
		return (-1);
	if (pthread_mutex_init(&info->mutex, NULL) < 0)
		return (-1);
	info->start = ptime();
	info->n_fed = 0;
	info->done = 0;
	i = 0;
	while (i < info->count)
	{
		info->seats[i].info = info;
		if (pthread_mutex_init(&info->seats[i].mutex, NULL) < 0)
			break ;
		info->seats[i].index = i;
		info->seats[i].flag = 0;
		info->seats[i].t_eat = info->start;
		info->seats[i].n_eat = 0;
		i += 1;
	}
	return (i);
}

static void
	ploop(t_info *info)
{
	int		i;
	long	time;

	while (plock(info))
	{
		i = 0;
		time = ptime();
		while (i < info->count)
		{
			if (info->seats[i].t_eat + info->t_die <= time)
			{
				plog(&info->seats[i], time, "died");
				info->done = 1;
				break ;
			}
			i += 1;
		}
		pthread_mutex_unlock(&info->mutex);
		if (!psleep(info, 1000))
			return ;
	}
}

static int
	prun(t_info *info)
{
	t_seat	*seat;
	int		i;
	int		j;

	i = 0;
	while (i < info->count)
	{
		seat = &info->seats[i];
		if (pthread_create(&seat->thread, NULL, pstart, seat) < 0)
			break ;
		i += 1;
	}
	ploop(info);
	j = 0;
	while (j < i)
	{
		seat = &info->seats[j];
		pthread_join(seat->thread, NULL);
		j += 1;
	}
	if (i != info->count)
		return (-1);
	return (0);
}

static void
	pexit(t_info *info, int count)
{
	int	i;

	if (count > -1)
		pthread_mutex_destroy(&info->mutex);
	i = 0;
	while (i < count)
	{
		pthread_mutex_destroy(&info->seats[i].mutex);
		i += 1;
	}
	free(info->seats);
}

int
	main(int argc, char **argv)
{
	t_info	info;
	int		count;
	int		status;

	if (argc < 5 || argc > 6)
		return (EXIT_FAILURE);
	info.count = patoi(argv[1]);
	info.t_die = patoi(argv[2]) * 1000;
	info.t_eat = patoi(argv[3]) * 1000;
	info.t_slp = patoi(argv[4]) * 1000;
	info.n_eat = 0;
	if (argc >= 6)
		info.n_eat = patoi(argv[5]);
	if (info.t_die < 0 || info.t_eat < 0 || info.t_slp < 0)
		return (EXIT_FAILURE);
	if (info.count < 0 || info.n_eat < 0)
		return (EXIT_FAILURE);
	count = pinit(&info);
	if (count == info.count)
		status = prun(&info);
	pexit(&info, count);
	if (count != info.count || status < 0)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
