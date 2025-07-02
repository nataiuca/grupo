/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils_2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzolotar <mzolotar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 08:16:29 by mzolotar          #+#    #+#             */
/*   Updated: 2025/06/30 08:22:17 by mzolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "minishell.h"

bool init_exec_vals(t_program *program, t_all *all)
{
	all->here->base_cwd = ft_strdup(program->prompt_cwd);
	if (!all->here->base_cwd)
	{
		ft_error(program, MSG_ERR_MALLOC, NULL, 1);
		return (false);
	}
	all->exec->num_pipes= count_type_tokens(all->tokens, pipe_type);
	all->exec->num_cmds = count_type_tokens(all->tokens, command);
	all->tokens_head = all->tokens;
	return (true);
}

bool must_use_env_builtin(char *cmd)
{
	if (!cmd)
		return (false);
	if (ft_strcmp(cmd, "env") == 0) // Solo si el comando es exactamente "env"
		return (true);
	return (false);
}

bool is_forbidden_env_path(char *cmd)
{
	if (!cmd)
		return false;
	return (
		ft_strcmp(cmd, "/usr/bin/env") == 0 ||
		ft_strcmp(cmd, "/bin/env") == 0 ||
		ft_strcmp(cmd, "./env") == 0 ||
		(ft_strcmp(cmd, "env") == 0 && !check_builtin(cmd)));
}

bool must_use_pwd_builtin(char *cmd)
{
	if (!cmd)
		return (false);
	if (ft_strcmp(cmd, "pwd") == 0) 	// Solo si el comando es exactamente "env"
		return (true);
	return (false);
}

bool is_forbidden_pwd_path(char *cmd)
{
	if (!cmd)
		return false;
	return (
		ft_strcmp(cmd, "/usr/bin/pwd") == 0 ||
		ft_strcmp(cmd, "/bin/pwd") == 0 ||
		ft_strcmp(cmd, "./pwd") == 0 ||
		(ft_strcmp(cmd, "pwd") == 0 && !check_builtin(cmd))
	);
}
