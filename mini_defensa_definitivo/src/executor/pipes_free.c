/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_free.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzolotar <mzolotar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 11:56:34 by mzolotar          #+#    #+#             */
/*   Updated: 2025/07/10 10:43:14 by mzolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	close_all_pipe_fds_in_parent(t_exec *exec)
{
	int	i;

	i = 0;
	if (!exec->pipe_fds)
		return ;
	while (i < exec->num_pipes)
	{
		if (exec->pipe_fds[i][0] != -1 && exec->pipe_fds[i][0] > 2)
			close(exec->pipe_fds[i][0]);
		if (exec->pipe_fds[i][1] != -1 && exec->pipe_fds[i][1] > 2)
			close(exec->pipe_fds[i][1]);
		i++;
	}
}

void	free_all_pipes(t_exec *exec)
{
	int	i;

	if (!exec->pipe_fds)
		return ;
	i = 0;
	while (i < exec->num_pipes)
	{
		if (exec->pipe_fds[i])
		{
			if (exec->pipe_fds[i][0] != -1)
				close(exec->pipe_fds[i][0]);
			if (exec->pipe_fds[i][1] != -1)
				close(exec->pipe_fds[i][1]);
			free(exec->pipe_fds[i]);
		}
		i++;
	}
	free(exec->pipe_fds);
	exec->pipe_fds = NULL;
}

void	free_pipes_up_to_index(t_exec *exec, int limit)
{
	int	i;

	if (!exec->pipe_fds)
		return ;
	i = 0;
	while (i < limit)
	{
		if (exec->pipe_fds[i])
		{
			if (exec->pipe_fds[i][0] != -1)
				close(exec->pipe_fds[i][0]);
			if (exec->pipe_fds[i][1] != -1)
				close(exec->pipe_fds[i][1]);
			free(exec->pipe_fds[i]);
		}
		i++;
	}
	free(exec->pipe_fds);
	exec->pipe_fds = NULL;
}

void	close_unused_pipes_in_parent(t_exec *exec, int i)
{
	if (i > 0)
	{
		if (exec->pipe_fds[i - 1][0] > 2)
			close(exec->pipe_fds[i - 1][0]);
	}
	if (i < exec->num_pipes)
	{
		if (exec->pipe_fds[i][1] > 2)
			close(exec->pipe_fds[i][1]);
	}
}

void	close_unused_pipes(t_exec *exec, int curr_cmd_idx)
{
	int	i;

	i = 0;
	while (i < exec->num_pipes)
	{
		if (i != curr_cmd_idx - 1 && exec->pipe_fds[i][0] > 2)
			close(exec->pipe_fds[i][0]);
		if (i != curr_cmd_idx && exec->pipe_fds[i][1] > 2)
			close(exec->pipe_fds[i][1]);
		i++;
	}
}
