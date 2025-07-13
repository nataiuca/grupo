/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natferna <natferna@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 11:56:48 by mzolotar          #+#    #+#             */
/*   Updated: 2025/07/03 19:21:16 by natferna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	init_pipe(t_program *program, t_exec *exec, int i)
{
	if (pipe(exec->pipe_fds[i]) == -1)
	{
		free_pipes_up_to_index(exec, i + 1);
		ft_error(program, MSG_ERR_PIPE, NULL, 1);
		return (-1);
	}
	return (0);
}

int	alloc_pipe_fd_array(t_program *program, t_exec *exec)
{
	exec->pipe_fds = malloc(sizeof(int *) * exec->num_pipes);
	if (!exec->pipe_fds)
	{
		ft_error(program, MSG_ERR_MALLOC, NULL, 1);
		return (-1);
	}
	return (0);
}

int	alloc_pipe_pair_at_index(t_program *program, t_exec *exec, int i)
{
	exec->pipe_fds[i] = malloc(sizeof(int) * 2);
	if (!exec->pipe_fds[i])
	{
		free_pipes_up_to_index(exec, i);
		ft_error(program, MSG_ERR_MALLOC, NULL, 1);
		return (-1);
	}
	return (0);
}

bool	init_all_pipes(t_program *program, t_all *all)
{
	int	i;

	i = 0;
	if (alloc_pipe_fd_array(program, all->exec) == -1)
		return (false);
	while (i < all->exec->num_pipes)
	{
		if (alloc_pipe_pair_at_index(program, all->exec, i) == -1)
		{
			free_pipes_up_to_index(all->exec, i);
			return (false);
		}
		if (init_pipe(program, all->exec, i) == -1)
		{
			free_pipes_up_to_index(all->exec, i + 1);
			return (false);
		}
		i++;
	}
	return (true);
}
