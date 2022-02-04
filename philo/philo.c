/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   phil.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: csteenvo <csteenvo@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/01 14:30:15 by csteenvo      #+#    #+#                 */
/*   Updated: 2022/02/03 14:12:58 by csteenvo      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdio.h>

void
	philo_log_unlocked(t_philo *philo, const char *str)
{
	unsigned long	num;
	unsigned long	time;

	if (philo->prog->run)
	{
		time = philo->prog->time - philo->prog->start;
		num = philo->index + 1;
		printf("%lu %lu %s\n", time / 1000, num, str);
		fflush(stdout);
	}
}

unsigned long
	philo_log(t_philo *philo, const char *str)
{
	unsigned long	time;

	pthread_mutex_lock(&philo->prog->mutex);
	time = time_time();
	philo->prog->time = time;
	philo_log_unlocked(philo, str);
	pthread_mutex_unlock(&philo->prog->mutex);
	return (time);
}

int
	philo_eat(t_philo *philo, size_t i, size_t j)
{
	pthread_mutex_lock(&philo->prog->philos[i].mutex);
	philo_log(philo, "has taken a fork");
	if (i == j)
	{
		pthread_mutex_unlock(&philo->prog->philos[i].mutex);
		return (0);
	}
	pthread_mutex_lock(&philo->prog->philos[j].mutex);
	pthread_mutex_lock(&philo->prog->mutex);
	philo->prog->time = time_time();
	philo_log_unlocked(philo, "has taken a fork");
	philo_log_unlocked(philo, "is eating");
	philo->n_eat += 1;
	if (philo->n_eat == philo->prog->n_eat)
		philo->prog->ate += 1;
	if (philo->prog->ate == philo->prog->count)
		philo->prog->run = 0;
	philo->t_eat = philo->prog->time;
	pthread_mutex_unlock(&philo->prog->mutex);
	time_wait(philo->t_eat + philo->prog->t_eat);
	return (1);
}

void
	*philo_run(void *arg)
{
	t_philo			*philo;
	size_t			i;
	size_t			j;

	philo = arg;
	i = (philo->index + (1 - philo->index % 2)) % philo->prog->count;
	j = (philo->index + philo->index % 2) % philo->prog->count;
	philo_log(philo, "is thinking");
	time_sleep(5000 * (philo->index % 2));
	while (philo->prog->run && philo_eat(philo, i, j))
	{
		philo->t_sleep = philo_log(philo, "is sleeping");
		pthread_mutex_unlock(&philo->prog->philos[i].mutex);
		pthread_mutex_unlock(&philo->prog->philos[j].mutex);
		time_wait(philo->t_sleep + philo->prog->t_sleep);
		philo_log(philo, "is thinking");
	}
	return (NULL);
}
