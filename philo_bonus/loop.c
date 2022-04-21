/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   loop.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: csteenvo <csteenvo@student.codam.n>          +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/04/21 13:34:24 by csteenvo      #+#    #+#                 */
/*   Updated: 2022/04/21 14:23:31 by csteenvo      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <signal.h>

void
	pstop(t_info *info)
{
	int	i;

	i = 0;
	while (i < info->count)
	{
		if (info->pids[i] != -1)
		{
			kill(info->pids[i], SIGTERM);
			waitpid(info->pids[i], NULL, 0);
		}
		i += 1;
	}
	free(info->pids);
	exit(EXIT_SUCCESS);
}

void
	*pswaiter(void *ptr)
{
	t_info	*info;
	int		index;

	info = ptr;
	index = 0;
	while (index < info->count)
	{
		psem(info->fed, 1);
		index += 1;
		if (index < info->count)
			psem(info->mutex, 0);
	}
	info->stopping = 1;
	pstop(info);
	return (NULL);
}

void
	ppwaiter(t_info *info)
{
	pid_t	pid;
	int		status;
	int		i;

	while (1)
	{
		pid = waitpid(0, &status, 0);
		if (pid < 0 && errno == EINTR)
			continue ;
		if (pid < 0 || WEXITSTATUS(status) == -1 || info->stopping)
			psem(info->mutex, 1);
		if (pid < 0)
			break ;
		i = 0;
		while (i < info->count)
		{
			if (info->pids[i] == pid)
				info->pids[i] = -1;
			i += 1;
		}
		break ;
	}
	pstop(info);
}

void
	ploop(t_info *info)
{
	if (pthread_create(&info->thread, NULL, pswaiter, info))
		exit(EXIT_FAILURE);
	if (pthread_detach(info->thread))
		exit(EXIT_FAILURE);
	ppwaiter(info);
}
