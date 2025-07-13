/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzolotar <mzolotar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 08:15:48 by mzolotar          #+#    #+#             */
/*   Updated: 2025/07/10 10:45:34 by mzolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_fork_error_2(t_program *program, t_all *all)
{
	ft_error(program, MSG_ERR_FORK, NULL, 1);
	free_child(program, all);
	exit(1);
}

void	exec_and_exit_child(t_all *all, t_tokens *curr, t_program *program,
		int i)
{
	int	builtin_exit_code;

	handle_check_child_result(all, program, i);
	if ((is_forbidden_env_path(curr->content))
		|| (is_forbidden_pwd_path(curr->content)))
	{
		ft_error(program, "external call not allowed", curr->content, 127);
		free_and_exit_child(all, program, 127);
	}
	if (check_builtin(curr->content) || must_use_env_builtin(curr->content)
		|| must_use_pwd_builtin(curr->content))
	{
		if (!apply_cmd_redir(all->exec, i, program))
			close_and_exit(all, i, program, 1);
		close_unused_pipes(all->exec, i);
		builtin_exit_code = ft_builtin(all, program);
		free_and_exit_child(all, program, builtin_exit_code);
	}
	else
	{
		if (!apply_cmd_redir(all->exec, i, program))
			close_and_exit(all, i, program, 1);
		close_unused_pipes(all->exec, i);
		execute_command(all, program, curr);
	}
}

int	check_child_fork(t_program *program, int i, t_all *all)
{
	if (all->exec->args && ft_strcmp(all->exec->args[0], ":") == 0)
	{
		if (!apply_redir(all, program))
			return (1);
		return (2);
	}
	if (!apply_redir(all, program) || !apply_cmd_redir(all->exec, i, program))
		return (1);
	return (0);
}

void	ft_exec(t_all *all, t_program *program)
{
	if (!check_here_doc(all, program, all->here))
		return ;
	if (!all->tokens || !program)
		return ;
	if (!update_args(all->exec, all->tokens, program))
		return ;
	if (all->exec->num_pipes == 0)
	{
		run_single_command(all, program);
		return ;
	}
	else
	{
		if (executor_loop(all, program) == -1)
			return ;
	}
}
