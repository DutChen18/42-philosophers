/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   time.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: csteenvo <csteenvo@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/01 14:05:57 by csteenvo      #+#    #+#                 */
/*   Updated: 2022/02/03 10:27:08 by csteenvo      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <sys/time.h>
#include <unistd.h>

unsigned long
	time_time(void)
{
	struct timeval	tp;

	gettimeofday(&tp, NULL);
	return (tp.tv_sec * 1000000 + tp.tv_usec);
}

int
	time_wait(unsigned long until)
{
	unsigned long	now;
	int				loops;

	loops = 0;
	while (1)
	{
		now = time_time();
		if (until <= now)
			break ;
		else if (until - now >= 20000)
			usleep(until - now - 5000);
		else if (until - now >= 1000)
			usleep((until - now) / 1.3);
		else
			usleep(until - now);
		loops += 1;
	}
	return (loops);
}

int
	time_sleep(unsigned long delta)
{
	return (time_wait(time_time() + delta));
}
