/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: csteenvo <csteenvo@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/01 12:31:38 by csteenvo      #+#    #+#                 */
/*   Updated: 2022/02/03 16:12:55 by csteenvo      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdlib.h>
#include <stdio.h>

static int
	ft_atoi(const char *str)
{
	long	value;

	value = 0;
	while (*str != '\0')
	{
		if (*str < '0' && *str > '9')
			return (-1);
		value = value * 10 + *str - '0';
		str += 1;
	}
	return (value);
}

static void
	init(t_prog *prog)
{
	size_t	i;
	t_philo	*philo;

	pthread_mutex_init(&prog->mutex, NULL);
	i = 0;
	while (i < prog->count)
	{
		prog->philos[i].prog = prog;
		prog->philos[i].index = i;
		prog->philos[i].t_eat = prog->start;
		prog->philos[i].t_sleep = prog->start;
		prog->philos[i].n_eat = 0;
		pthread_mutex_init(&prog->philos[i].mutex, NULL);
		i += 1;
	}
	i = 0;
	while (i < prog->count)
	{
		philo = &prog->philos[i];
		pthread_create(&philo->thread, NULL, philo_run, philo);
		i += 1;
	}
}

static void
	run(t_prog *prog)
{
	size_t	i;

	while (prog->run)
	{
		time_sleep(1000);
		i = 0;
		pthread_mutex_lock(&prog->mutex);
		prog->time = time_time();
		while (prog->run && i < prog->count)
		{
			if (prog->time - prog->philos[i].t_eat > prog->t_die)
			{
				philo_log_unlocked(&prog->philos[i], "died");
				prog->run = 0;
			}
			i += 1;
		}
		pthread_mutex_unlock(&prog->mutex);
	}
}

static void
	stop(t_prog *prog)
{
	size_t	i;

	i = 0;
	while (i < prog->count)
	{
		pthread_join(prog->philos[i].thread, NULL);
		i += 1;
	}
	while (i < prog->count)
	{
		pthread_mutex_destroy(&prog->philos[i].mutex);
		i += 1;
	}
	pthread_mutex_destroy(&prog->mutex);
}

int
	main(int argc, char **argv)
{
	t_prog	prog;

	prog.start = time_time();
	prog.time = prog.start;
	prog.count = ft_atoi(argv[1]);
	prog.ate = 0;
	prog.t_die = (unsigned long) ft_atoi(argv[2]) * 1000;
	prog.t_eat = (unsigned long) ft_atoi(argv[3]) * 1000;
	prog.t_sleep = (unsigned long) ft_atoi(argv[4]) * 1000;
	if (argc == 5)
		prog.n_eat = -1;
	else
		prog.n_eat = ft_atoi(argv[5]);
	prog.philos = malloc(prog.count * sizeof(*prog.philos));
	prog.run = 1;
	init(&prog);
	run(&prog);
	stop(&prog);
	return (EXIT_SUCCESS);
}
