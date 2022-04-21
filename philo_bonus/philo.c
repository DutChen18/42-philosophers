/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philo.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: csteenvo <csteenvo@student.codam.n>          +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/03/31 10:40:02 by csteenvo      #+#    #+#                 */
/*   Updated: 2022/04/21 14:31:51 by csteenvo      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdlib.h>
#include <unistd.h>

void
	ptake(t_info *info)
{
	psem(info->forks, 1);
	psem(info->mutex, 1);
	info->now = ptime();
	pputs(info, "has taken a fork");
	psem(info->mutex, 0);
	psem(info->forks, 1);
	psem(info->mutex, 1);
	info->now = ptime();
	pputs(info, "has taken a fork");
	pputs(info, "is eating");
	info->p_t_eat = info->now;
	info->p_n_eat += 1;
	if (info->p_n_eat == info->n_eat)
		psem(info->fed, 0);
	else
		psem(info->mutex, 0);
}

void
	*pmon(void *ptr)
{
	t_info	*info;

	info = ptr;
	while (1)
	{
		psem(info->mutex, 1);
		info->now = ptime();
		if (info->now - info->p_t_eat >= info->t_die)
		{
			pputs(info, "died");
			exit(0);
		}
		psem(info->mutex, 0);
		psleep(1000);
	}
}

void
	pstart(t_info *info)
{
	if (pthread_create(&info->thread, NULL, pmon, info))
		exit(-1);
	if (pthread_detach(info->thread))
		exit(-1);
	if (info->index % 2 == 1)
		psleep(10000);
	while (1)
	{
		psem(info->mutex, 1);
		info->now = ptime();
		pputs(info, "is thinking");
		psem(info->mutex, 0);
		ptake(info);
		psleep(info->t_eat);
		psem(info->forks, 0);
		psem(info->forks, 0);
		psem(info->mutex, 1);
		info->now = ptime();
		pputs(info, "is sleeping");
		psem(info->mutex, 0);
		psleep(info->t_slp);
	}
}

void
	pputnum(unsigned long num)
{
	char	c;

	if (num == 0)
	{
		write(STDOUT_FILENO, "0", 1);
		return ;
	}
	c = num % 10 + '0';
	if (num >= 10)
		pputnum(num / 10);
	write(STDOUT_FILENO, &c, 1);
}

void
	pputs(t_info *info, const char *str)
{
	const long	time = (info->now - info->start) / 1000;
	size_t		size;

	pputnum(time);
	write(STDOUT_FILENO, " ", 1);
	pputnum(info->index + 1);
	write(STDOUT_FILENO, " ", 1);
	size = 0;
	while (str[size] != '\0')
		size += 1;
	write(STDOUT_FILENO, str, size);
	write(STDOUT_FILENO, "\n", 1);
}
