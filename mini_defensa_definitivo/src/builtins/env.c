/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzolotar <mzolotar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 12:44:15 by mzolotar          #+#    #+#             */
/*   Updated: 2025/06/30 08:32:15 by mzolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	update_env_var_if_exists(t_env *env, const char *key,
			const char *value)
{
	while (env)
	{
		if (ft_strcmp(env->key, key) == 0)
		{
			free(env->value);
			env->value = ft_strdup(value);
			return (1);
		}
		env = env->next;
	}
	return (0);
}

static	void	write_env(t_env *env, int out_fd)
{
	if (!env)
		return ;
	while (env)
	{
		if (env->key && env->value != NULL)
		{
			ft_putstr_fd(env->key, out_fd);
			ft_putchar_fd('=', out_fd);
			ft_putendl_fd(env->value, out_fd);
		}
		env = env->next;
	}
}

static	bool	has_valid_path(t_env *env)
{
	while (env)
	{
		if (ft_strcmp(env->key, "PATH") == 0)
			return (env->value && env->value[0] != '\0');
		env = env->next;
	}
	return (false);
}

static	int	check_env_error(t_program *program, char **args)
{
	if (!args[1])
		return (0);
	if (args[1][0] == '-')
	{
		ft_putstr_fd("env: invalid option -- '", STDERR_FILENO);
		ft_putstr_fd(&args[1][1], STDERR_FILENO);
		ft_putstr_fd("'\n", STDERR_FILENO);
		ft_putstr_fd("Try 'env' with no options.\n", STDERR_FILENO);
		program->last_exit_status = 1;
		return (1);
	}
	else
	{
		ft_putstr_fd("env: ‘", STDERR_FILENO);
		ft_putstr_fd(args[1], STDERR_FILENO);
		ft_putstr_fd("’: No such file or directory\n", STDERR_FILENO);
		program->last_exit_status = 127;
		return (127);
	}
}

int	ft_env(t_program *program, t_all *all, char **args)
{
	int	out;
	int	error_code;

	error_code = check_env_error(program, args);
	if (error_code != 0)
		return (error_code);
	if (!has_valid_path(program->env))
	{
		ft_error(program, MSG_ERR_NO_SUCH_FILE, "env", 127);
		return (127);
	}
	if (all->exec->outfile != -1)
		out = all->exec->outfile;
	else
		out = STDOUT_FILENO;
	write_env(program->env, out);
	program->last_exit_status = 0;
	return (0);
}
