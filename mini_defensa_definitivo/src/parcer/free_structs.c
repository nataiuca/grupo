/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_structs.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzolotar <mzolotar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 14:49:10 by mzolotar          #+#    #+#             */
/*   Updated: 2025/07/06 10:00:20 by mzolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_exec(t_exec *exec)
{
	if (!exec)
		return ;
	cleanup_exec(exec);
	free(exec);
}

void	free_meta_struct(t_metachars *meta)
{
	if (!meta)
		return ;
	free_split_strs(meta->allow_op);
	free_split_strs(meta->var);
	free_split_strs(meta->unexpected_token);
	free(meta);
}

void	free_here(t_here *here, int mode)
{
	int	i;

	if (!here)
		return ;
	i = 0;
	while (i < HERE_DOCS_MAX)
	{
		if (here->fd_array[i] != -1)
			close(here->fd_array[i]);
		if (here->here_name_docs[i])
		{
			if (mode != FREE_CHILD)
				unlink(here->here_name_docs[i]);
			free(here->here_name_docs[i]);
			here->here_name_docs[i] = NULL;
		}
		i++;
	}
	if (here->base_cwd)
	{
		free(here->base_cwd);
		here->base_cwd = NULL;
	}
	free(here);
}

void	free_program(t_program *program)
{
	if (!program)
		return ;
	if (program)
	{
		if (program->env)
		{
			destroy_env_list(program->env);
			program->env = NULL;
		}
		if (program->envp_copy)
		{
			free_split_strs(program->envp_copy);
			program->envp_copy = NULL;
		}
		if (program->prompt_hostname)
			free(program->prompt_hostname);
	}
}

void	free_all_structs(t_all *all, int mode)
{
	free_all_exec_meta_tokens(all);
	if (all->token)
	{
		free_split_strs(all->token);
		all->token = NULL;
	}
	if (all->here)
	{
		free_here(all->here, mode);
		all->here = NULL;
	}
}
