/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philo.h                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: csteenvo <csteenvo@student.codam.n>          +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/03/31 10:39:54 by csteenvo      #+#    #+#                 */
/*   Updated: 2022/04/21 14:28:17 by csteenvo      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <semaphore.h>

typedef struct s_info	t_info;

struct s_info
{
	pthread_t	thread;
	sem_t		*forks;
	sem_t		*mutex;
	sem_t		*fed;
	int			*pids;
	int			stopping;
	int			count;
	int			index;
	long		start;
	long		now;
	long		t_die;
	long		t_eat;
	long		t_slp;
	int			n_eat;
	int			n_fed;
	long		p_t_eat;
	long		p_n_eat;
};

long	ptime(void);
void	psem(sem_t *sem, int state);
void	psleep(long delta);
long	patol(const char *str, long max);

void	pputnum(unsigned long num);
void	pputs(t_info *info, const char *str);

void	ptake(t_info *info);
void	*pmon(void *ptr);
void	pstart(t_info *info);

void	pstop(t_info *info);
void	*pswaiter(void *ptr);
void	ppwaiter(t_info *info);
void	ploop(t_info *info);

#endif
