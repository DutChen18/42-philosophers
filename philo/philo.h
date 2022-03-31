/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philo.h                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: csteenvo <csteenvo@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/04 14:45:58 by csteenvo      #+#    #+#                 */
/*   Updated: 2022/02/17 13:22:08 by csteenvo      ########   odam.nl         */
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
	int				flag;
	long			t_eat;
	int				n_eat;
};

struct s_info
{
	t_seat			*seats;
	pthread_mutex_t	mutex;
	int				count;
	long			start;
	long			t_die;
	long			t_eat;
	long			t_slp;
	int				n_eat;
	int				n_fed;
	int				done;
};

long	ptime(void);
int		plock(t_info *info);
long	pwait(t_info *info, long end);
long	psleep(t_info *info, long delta);
int		patoi(const char *str);

void	plog(t_seat *seat, long time, const char *str);
int		ptake1(t_seat *seat, t_seat *fork, int *forks);
int		ptake2(t_info *info, t_seat *seat);
void	pdrop2(t_info *info, t_seat *seat);
void	*pstart(void *ptr);

#endif
