/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzolotar <mzolotar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 23:46:03 by mzolotar          #+#    #+#             */
/*   Updated: 2025/07/13 11:15:41 by mzolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cleanup_exec_pipes(t_exec *exec)
{
	int	i;

	i = 0;
	if (exec->pipe_fds)
	{
		while (i < exec->num_pipes)
		{
			close(exec->pipe_fds[i][0]);
			close(exec->pipe_fds[i][1]);
			free(exec->pipe_fds[i]);
			i++;
		}
		free(exec->pipe_fds);
		exec->pipe_fds = NULL;
	}
}

void	cleanup_exec(t_exec *exec)
{
	cleanup_exec_pipes(exec);
	if (exec->args)
	{
		free_split_strs(exec->args);
		exec->args = NULL;
	}
	if (exec->pids)
	{
		free(exec->pids);
		exec->pids = NULL;
	}
	if (exec->outfile > 2)
	{
		close(exec->outfile);
		exec->outfile = -1;
	}
	if (exec->infile > 2)
	{
		close(exec->infile);
		exec->infile = -1;
	}
}

void	destroy_env_list(t_env *env)
{
	t_env	*tmp;

	while (env)
	{
		tmp = env->next;
		free(env->key);
		free(env->value);
		free(env);
		env = tmp;
	}
}

void	destroy_env_node(t_env *node)
{
	if (!node)
		return ;
	if (node->key)
		free(node->key);
	if (node->value)
		free(node->value);
	free(node);
}

void	free_all_exec_meta_tokens(t_all *all)
{
	if (all->line)
	{
		free(all->line);
		all->line = NULL;
	}
	if (all->exec)
	{
		free_exec(all->exec);
		all->exec = NULL;
	}
	if (all->meta)
	{
		free_meta_struct(all->meta);
		all->meta = NULL;
	}
	if (all->tokens)
	{
		free_tokens_list(&all->tokens);
		all->tokens = NULL;
	}
}
