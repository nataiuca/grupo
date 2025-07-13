/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzolotar <mzolotar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 07:21:05 by mzolotar          #+#    #+#             */
/*   Updated: 2025/07/13 08:41:12 by mzolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_valid_numeric_arg(char *arg)
{
	int	i;

	i = 0;
	if (!arg || arg[0] == '\0')
		return (0);
	if (arg[0] == '+' || arg[0] == '-')
		i++;
	if (arg[i] == '\0')
		return (0);
	while (arg[i])
	{
		if (!ft_isdigit(arg[i]))
			return (0);
		i++;
	}
	return (1);
}

static int	ft_aux_exit(t_exec *exec, t_program *program)
{
	if (exec->args[1])
	{
		if (!is_valid_numeric_arg(exec->args[1]))
		{
			ft_error(program, MSG_ERR_NOT_NUMERIC, exec->args[1], 2);
			return (2);
		}
		if (exec->args[2])
		{
			ft_error(program, MSG_ERR_TOO_MANY_ARGS, "exit", 1);
			return (-1);
		}
		return (ft_atoi(exec->args[1]) % 256);
	}
	return (0);
}

int	ft_exit(t_all *all, t_program *program)
{
	int	exit_code;

	if (all->exec->num_pipes == 0)
		ft_putendl_fd("exit", STDERR_FILENO);
	exit_code = ft_aux_exit(all->exec, program);
	if (exit_code == -1)
		return (program->last_exit_status);
	free_all_structs(all, 0);
	rl_clear_history();
	free_program(program);
	exit(exit_code);
}
