/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   test.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: csteenvo <csteenvo@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/03 09:31:58 by csteenvo      #+#    #+#                 */
/*   Updated: 2022/02/03 10:05:32 by csteenvo      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <unistd.h>
#include <stdio.h>
#include <limits.h>

#define TESTS 100

int
	main(void)
{
	unsigned long	start;
	unsigned long	time, total_time, min_time, max_time;
	int				loops, total_loops, min_loops, max_loops;
	size_t			i;

	i = 0;
	total_time = 0;
	min_time = ULONG_MAX;
	max_time = 0;
	total_loops = 0;
	min_loops = INT_MAX;
	max_loops = 0;
	while (i < TESTS)
	{
		start = time_time();
		loops = time_wait(start + 50000);
		time = time_time() - start;
		total_time += time;
		if (time < min_time)
			min_time = time;
		if (time > max_time)
			max_time = time;
		total_loops += loops;
		if (loops < min_loops)
			min_loops = loops;
		if (loops > max_loops)
			max_loops = loops;
		i += 1;
	}
	printf("min: %i %lu\n", min_loops, min_time);
	printf("max: %i %lu\n", max_loops, max_time);
	printf("avg: %f %lu\n", (float) total_loops / TESTS, total_time / TESTS);
	return (0);
}
