/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   time.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: csteenvo <csteenvo@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/01 14:05:57 by csteenvo      #+#    #+#                 */
/*   Updated: 2022/02/01 14:29:00 by csteenvo      ########   odam.nl         */
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

void
	time_wait(unsigned long until)
{
	while (time_time() < until)
		usleep(1000);
}

void
	time_sleep(unsigned long delta)
{
	time_wait(time_time() + delta);
}
