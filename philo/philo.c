/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   phil.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: csteenvo <csteenvo@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/01 14:30:15 by csteenvo      #+#    #+#                 */
/*   Updated: 2022/02/01 16:08:15 by csteenvo      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdio.h>

static int
	take(t_philo *philo, size_t i, size_t j)
{
	pthread_mutex_lock(&philo->prog->philos[i].mutex);
	philo_log(philo, "has taken a fork", 0);
	if (i == j)
	{
		pthread_mutex_unlock(&philo->prog->philos[i].mutex);
		return (1);
	}
	pthread_mutex_lock(&philo->prog->philos[j].mutex);
	philo_log(philo, "has taken a fork", 0);
	return (0);
}

unsigned long
	philo_log(t_philo *philo, const char *str, int stop)
{
	unsigned long	num;
	unsigned long	time;

	pthread_mutex_lock(&philo->prog->mutex);
	time = time_time();
	if (philo->prog->run)
	{
		num = philo->index + 1;
		printf("%lu %lu %s\n", (time - philo->prog->start) / 1000, num, str);
		fflush(stdout);
		philo->prog->run = !stop;
	}
	pthread_mutex_unlock(&philo->prog->mutex);
	return (time);
}

void
	*philo_run(void *arg)
{
	t_philo	*philo;
	size_t	i;
	size_t	j;

	philo = arg;
	i = (philo->index + (1 - philo->index % 2)) % philo->prog->count;
	j = (philo->index + philo->index % 2) % philo->prog->count;
	while (philo->prog->run)
	{
		philo_log(philo, "is thinking", 0);
		if (take(philo, i, j))
			break ;
		philo->n_eat += 1;
		philo->t_eat = philo_log(philo, "is eating", 0);
		time_wait(philo->t_eat + philo->prog->t_eat);
		pthread_mutex_unlock(&philo->prog->philos[i].mutex);
		pthread_mutex_unlock(&philo->prog->philos[j].mutex);
		time_wait(philo_log(philo, "is sleeping", 0) + philo->prog->t_sleep);
	}
	return (NULL);
}
