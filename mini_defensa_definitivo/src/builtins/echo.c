/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzolotar <mzolotar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 07:31:19 by mzolotar          #+#    #+#             */
/*   Updated: 2025/07/10 09:10:32 by mzolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	echo_flag(char *str)
{
	int	i;

	if (str[0] != '-')
		return (0);
	i = 1;
	while (str[i])
	{
		if (str[i] != 'n')
			return (0);
		i++;
	}
	if (i == 1)
		return (0);
	return (1);
}

static bool	check_arg_max(t_program *program, char **args, int start_index)
{
	int		total_len;
	int		i;
	char	limit_str[12];

	i = start_index;
	total_len = 0;
	while (args[i])
	{
		total_len += ft_strlen(args[i]);
		if (args[i + 1])
			total_len += 1;
		i++;
	}
	if (total_len >= ARG_MAX_ECHO)
	{
		ft_itoa_without_malloc(ARG_MAX_ECHO, limit_str, sizeof(limit_str));
		ft_error(program, MSG_ERR_ARG_LEN, limit_str, 0);
		return (false);
	}
	return (true);
}

static int	skip_echo_flags(char **args, int *print_newline)
{
	int	i;

	i = 1;
	while (args[i] && echo_flag(args[i]))
	{
		*print_newline = true;
		i++;
	}
	return (i);
}

static void	print_echo_args(char **args, int i, int out)
{
	while (args[i])
	{
		ft_putstr_fd(args[i], out);
		if (args[i + 1])
			ft_putchar_fd(' ', out);
		i++;
	}
}

int	ft_echo(t_program *program, t_exec *exec)
{
	int		i;
	int		out;
	int		print_newline;

	print_newline = 0;
	if (exec->outfile != -1)
		out = exec->outfile;
	else
		out = STDOUT_FILENO;
	i = skip_echo_flags(exec->args, &print_newline);
	if (!check_arg_max(program, exec->args, i))
		return (1);
	print_echo_args(exec->args, i, out);
	if (!print_newline)
		ft_putchar_fd('\n', out);
	program->last_exit_status = 0;
	return (0);
}
