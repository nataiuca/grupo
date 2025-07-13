/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzolotar <mzolotar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 11:13:33 by mzolotar          #+#    #+#             */
/*   Updated: 2025/06/30 08:31:19 by mzolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	check_builtin(char *cmd)
{
	if (!cmd)
		return (false);
	if (ft_strcmp(cmd, "echo") == 0
		|| ft_strcmp(cmd, "cd") == 0
		|| ft_strcmp(cmd, "pwd") == 0
		|| ft_strcmp(cmd, "export") == 0
		|| ft_strcmp(cmd, "unset") == 0
		|| ft_strcmp(cmd, "env") == 0
		|| ft_strcmp(cmd, "exit") == 0)
		return (true);
	return (false);
}

int	ft_builtin(t_all *all, t_program *program)
{
	char	*cmd;
	int		exit_code;

	exit_code = 0;
	cmd = all->exec->args[0];
	if (ft_strcmp(cmd, "echo") == 0)
		exit_code = ft_echo(program, all->exec);
	else if (ft_strcmp(cmd, "cd") == 0)
		exit_code = ft_cd(all, program);
	else if (ft_strcmp(cmd, "exit") == 0)
	{
		exit_code = ft_exit(all, program);
		return (program->last_exit_status);
	}
	else if (ft_strcmp(cmd, "pwd") == 0)
		exit_code = ft_pwd(all, program, all->exec->args);
	else if (ft_strcmp(cmd, "env") == 0)
		exit_code = ft_env(program, all, all->exec->args);
	else if (ft_strncmp(cmd, "export", 6) == 0)
		exit_code = ft_export(all, all->exec->args, program);
	else if (ft_strncmp(cmd, "unset", 5) == 0)
		exit_code = ft_unset(all, all->exec->args, program);
	return (exit_code);
}
