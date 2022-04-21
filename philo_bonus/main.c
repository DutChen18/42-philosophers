/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: csteenvo <csteenvo@student.codam.n>          +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/03/31 15:53:37 by csteenvo      #+#    #+#                 */
/*   Updated: 2022/04/21 14:15:31 by csteenvo      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>

#include <stdio.h>

int
	pinitsem(t_info *info)
{
	info->mutex = sem_open("mutex.sem",
			O_CREAT | O_EXCL, 0600, 1);
	if (info->mutex != NULL)
	{
		sem_unlink("mutex.sem");
		info->forks = sem_open("forks.sem",
				O_CREAT | O_EXCL, 0600, info->count);
		if (info->forks != NULL)
		{
			sem_unlink("forks.sem");
			info->fed = sem_open("fed.sem",
					O_CREAT | O_EXCL, 0600, info->count);
			if (info->fed != NULL)
			{
				sem_unlink("fed.sem");
				return (0);
			}
			sem_close(info->forks);
		}
		sem_close(info->mutex);
	}
	return (1);
}

int
	pinit(t_info *info)
{
	int	i;

	info->stopping = 0;
	info->pids = malloc(sizeof(*info->pids) * info->count);
	if (info->pids != NULL)
	{
		i = 0;
		while (i < info->count)
		{
			info->pids[i] = -1;
			i += 1;
		}
		if (!pinitsem(info))
		{
			i = 0;
			while (i < info->count)
			{
				psem(info->fed, 1);
				i += 1;
			}
			return (0);
		}
		free(info->pids);
	}
	return (1);
}

void
	prun(t_info *info)
{
	if (!pinit(info))
	{
		info->start = ptime();
		info->index = 0;
		while (info->index < info->count)
		{
			info->p_t_eat = info->start;
			info->p_n_eat = 0;
			info->pids[info->index] = fork();
			if (info->pids[info->index] < 0)
				break ;
			if (info->pids[info->index] == 0)
				pstart(info);
			info->index += 1;
		}
		if (info->index == info->count)
			ploop(info);
	}
}

int
	main(int argc, char **argv)
{
	t_info	info;

	if (argc < 5 || argc > 6)
		return (EXIT_FAILURE);
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
