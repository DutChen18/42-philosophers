/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philo.h                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: csteenvo <csteenvo@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/01 12:07:34 by csteenvo      #+#    #+#                 */
/*   Updated: 2022/02/01 14:42:54 by csteenvo      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>

typedef struct s_philo	t_philo;
typedef struct s_prog	t_prog;

struct s_philo
{
	t_prog			*prog;
	size_t			index;
	unsigned long	t_eat;
	int				n_eat;
	pthread_t		thread;
	pthread_mutex_t	mutex;
};

struct s_prog
{
	t_philo			*philos;
	unsigned long	start;
	size_t			count;
	unsigned long	t_die;
	unsigned long	t_eat;
	unsigned long	t_sleep;
	int				n_eat;
	int				run;
	pthread_mutex_t	mutex;
};

unsigned long	time_time(void);
void			time_wait(unsigned long until);
void			time_sleep(unsigned long delta);

unsigned long	philo_log(t_philo *philo, const char *str, int stop);
void			*philo_run(void *arg);

#endif
