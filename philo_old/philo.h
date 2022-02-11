/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philo.h                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: csteenvo <csteenvo@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/01 12:07:34 by csteenvo      #+#    #+#                 */
/*   Updated: 2022/02/03 13:18:09 by csteenvo      ########   odam.nl         */
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
	unsigned long	t_sleep;
	int				n_eat;
	pthread_t		thread;
	pthread_mutex_t	mutex;
};

struct s_prog
{
	t_philo			*philos;
	unsigned long	start;
	unsigned long	time;
	size_t			count;
	size_t			ate;
	unsigned long	t_die;
	unsigned long	t_eat;
	unsigned long	t_sleep;
	int				n_eat;
	int				run;
	pthread_mutex_t	mutex;
};

unsigned long	time_time(void);
int				time_wait(unsigned long until);
int				time_sleep(unsigned long delta);

void			philo_log_unlocked(t_philo *philo, const char *str);
unsigned long	philo_log(t_philo *philo, const char *str);
int				philo_eat(t_philo *philo, size_t i, size_t j);
void			*philo_run(void *arg);

#endif
