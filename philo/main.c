/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: csteenvo <csteenvo@student.codam.n>          +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/03/31 15:53:37 by csteenvo      #+#    #+#                 */
/*   Updated: 2022/03/31 16:28:52 by csteenvo      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdlib.h>

void
	start(t_info *info)
{
	int	i;
	int	done;

	info->seats = malloc(sizeof(*info->seats) * info->count);
	pthread_mutex_init(&info->mutex, NULL);
	i = 0;
	while (i < info->count)
	{
		info->seats[i].info = info;
		info->seats[i].index = i;
		pthread_mutex_init(&info->seats[i].mutex, NULL);
		info->seats[i].n_eat = 0;
		info->seats[i].state = 0;
		i += 1;
	}
	info->start = ptime();
	i = 0;
	while (i < info->count)
	{
		info->seats[i].t_eat = info->start;
		pthread_create(&info->seats[i].thread, NULL, pstart, &info->seats[i]);
		pthread_detach(info->seats[i].thread);
		i += 1;
	}
	done = 0;
	while (!done)
	{
		pthread_mutex_lock(&info->mutex);
		done = info->done;
		pthread_mutex_unlock(&info->mutex);
	}
}

int
	main(void)
{
	t_info	info;

	info.count = 10;
	info.done = 0;
	info.n_eat = 5;
	info.n_fed = 0;
	info.t_die = 800000;
	info.t_eat = 200000;
	info.t_slp = 200000;
	start(&info);
}
