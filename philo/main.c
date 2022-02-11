/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: csteenvo <csteenvo@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/11 14:33:50 by csteenvo      #+#    #+#                 */
/*   Updated: 2022/02/11 15:10:21 by csteenvo      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdlib.h>
#include <stdio.h>

void
	pinit(t_info *info)
{
	int	i;

	info->seats = malloc(sizeof(*info->seats) * info->count);
	pthread_mutex_init(&info->mutex, NULL);
	info->start = ptime();
	info->n_fed = 0;
	info->done = 0;
	i = 0;
	while (i < info->count)
	{
		info->seats[i].info = info;
		pthread_mutex_init(&info->seats[i].mutex, NULL);
		info->seats[i].index = i;
		info->seats[i].flag = 0;
		info->seats[i].t_eat = info->start;
		info->seats[i].n_eat = 0;
		i += 1;
	}
}

void
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

void
	prun(t_info *info)
{
	int		i;

	i = 0;
	while (i < info->count)
	{
		pthread_create(&info->seats[i].thread, NULL, pstart, &info->seats[i]);
		i += 2;
	}
	psleep(info, 1000);
	i = 1;
	while (i < info->count)
	{
		pthread_create(&info->seats[i].thread, NULL, pstart, &info->seats[i]);
		i += 2;
	}
	ploop(info);
	i = 0;
	while (i < info->count)
	{
		pthread_join(info->seats[i].thread, NULL);
		i += 1;
	}
}

int
	main(int argc, char **argv)
{
	t_info	info;

	(void) argc;
	info.count = atoi(argv[1]);
	info.t_die = atoi(argv[2]) * 1000;
	info.t_eat = atoi(argv[3]) * 1000;
	info.t_slp = atoi(argv[4]) * 1000;
	info.n_eat = 0;
	pinit(&info);
	prun(&info);
}
