/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_single_cmd.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzolotar <mzolotar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 08:49:46 by mzolotar          #+#    #+#             */
/*   Updated: 2025/07/12 11:19:11 by mzolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_child(t_program *program, t_all *all)
{
	if (all->exec->args && ft_strcmp(all->exec->args[0], ":") == 0)
	{
		if (!apply_redir(all, program))
			return (1);
		return (2);
	}
	if (!apply_redir(all, program) || !config_fd_redir(all->exec->infile,
			all->exec->outfile, program))
		return (1);
	return (0);
}

static void	handle_parent_process(pid_t pid, t_program *program, int status)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, handler);
	waitpid(pid, &status, 0);
	if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGINT)
		{
			g_atomic = SIGINT;
			program->last_exit_status = 130;
		}
		else if (WTERMSIG(status) == SIGQUIT)
		{
			g_atomic = SIGQUIT;
			program->last_exit_status = 131;
		}
	}
	else
		program->last_exit_status = WEXITSTATUS(status);
	signal(SIGINT, handler_builtins);
	handle_signal_output(program);
	signal_handling();
}

void	fork_and_run_single_cmd(t_all *all, t_program *program, t_tokens *curr)
{
	pid_t	pid;
	int		status;
	int		check_child_result;

	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, handler_child);
	status = 0;
	pid = fork();
	if (pid < 0)
		return (ft_error(program, MSG_ERR_FORK, NULL, 1), (void)0);
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, handler_child);
		check_child_result = check_child(program, all);
		if (check_child_result == 1)
			free_and_exit_child(all, program, program->last_exit_status);
		if (check_child_result == 2)
			free_and_exit_child(all, program, 0);
		execute_command(all, program, curr);
		exit(EXIT_FAILURE);
	}
	handle_parent_process(pid, program, status);
}

void	execute_command(t_all *all, t_program *program, t_tokens *curr)
{
	char	*cmd_path;
	int		status;

	if (!curr || !curr->content || !update_args(all->exec, curr, program))
		exit(1);
	update_envp_copy(program);
	cmd_path = resolve_command_path(curr, all, program);
	if (!cmd_path)
	{
		free_and_exit_child(all, program, program->last_exit_status);
	}
	status = validate_executable(program, all, cmd_path);
	if (status != 0)
	{
		free(cmd_path);
		free_and_exit_child(all, program, program->last_exit_status);
	}
	if (execve(cmd_path, all->exec->args, program->envp_copy) != 0)
	{
		ft_error(program, MSG_ERR_CMD_NOT_FOUND, all->exec->args[0], 127);
		free(cmd_path);
		free_and_exit_child(all, program, 127);
	}
	free(cmd_path);
	free_and_exit_child(all, program, 0);
}

void	run_single_command(t_all *all, t_program *program)
{
	t_tokens	*first_cmd;

	first_cmd = find_cmd(all->tokens);
	if ((first_cmd && is_forbidden_env_path(first_cmd->content))
		|| (first_cmd && is_forbidden_pwd_path(first_cmd->content)))
	{
		ft_error(program, "external call not allowed", first_cmd->content, 127);
		return ;
	}
	if (first_cmd && (check_builtin(first_cmd->content)
			|| must_use_env_builtin(first_cmd->content)
			|| must_use_pwd_builtin(first_cmd->content)))
	{
		if (!apply_redir(all, program))
			return ;
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, handler);
		ft_builtin(all, program);
		signal(SIGINT, handler);
		signal(SIGQUIT, SIG_IGN);
		return ;
	}
	fork_and_run_single_cmd(all, program, first_cmd);
}
