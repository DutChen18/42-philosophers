/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philo.h                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: csteenvo <csteenvo@student.codam.n>          +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/03/31 10:39:54 by csteenvo      #+#    #+#                 */
/*   Updated: 2022/04/15 10:45:20 by csteenvo      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>

typedef struct s_seat	t_seat;
typedef struct s_info	t_info;

struct s_seat
{
	t_info			*info;
	pthread_t		thread;
	pthread_mutex_t	mutex;
	int				index;
	int				state;
	long			t_eat;
	int				n_eat;
};

struct s_info
{
	t_seat			*seats;
	pthread_mutex_t	mutex;
	int				count;
	int				done;
	long			start;
	long			now;
	long			t_die;
	long			t_eat;
	long			t_slp;
	int				n_eat;
	int				n_fed;
};

long	ptime(void);
void	pputs(t_seat *seat, const char *str);
int		pcheck(t_seat *seat, const char *str);
void	psleep(t_info *info, long delta);
long	patol(const char *str, long max);

void	ptake(t_seat *seat, t_seat *fork, int *state, int *other);
void	ptake1(t_seat *seat, t_seat *fork, int *state, int *other);
int		ptake2(t_seat *seat, t_seat *next);
void	pdrop(t_seat *fork);
void	*pstart(void *ptr);

#endif
