/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philo.h                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: csteenvo <csteenvo@student.codam.n>          +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/03/31 10:39:54 by csteenvo      #+#    #+#                 */
/*   Updated: 2022/04/15 12:05:05 by csteenvo      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <semaphore.h>

typedef struct s_info	t_info;

struct s_info
{
	sem_t	*forks;
	sem_t	*mutex;
	int		count;
	int		index;
	long	start;
	long	now;
	long	t_die;
	long	t_eat;
	long	t_slp;
	int		n_eat;
	int		n_fed;
};

long	ptime(void);
void	pputs(t_info *info, const char *str);
void	pcheck(t_info *info, const char *str);
void	psleep(long delta);
long	patol(const char *str, long max);

void	ptake(t_seat *seat, t_seat *fork, int *state, int *other);
void	ptake1(t_seat *seat, t_seat *fork, int *state, int *other);
int		ptake2(t_seat *seat, t_seat *next);
void	pdrop(t_seat *fork);
void	*pstart(void *ptr);

#endif
