/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzolotar <mzolotar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 07:53:30 by mzolotar          #+#    #+#             */
/*   Updated: 2025/06/11 13:11:59 by mzolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
/*
void	write_error_message(const char *msg, const char *arg)
{
	if (arg)
	{
		if (ft_strcmp(msg, MSG_ERR_CMD_NOT_FOUND) == 0)
		{
			write(2, arg, ft_strlen(arg));
			write(2, ": ", 2);
		}
		else if (ft_strcmp(msg, MSG_ERR_NOT_IS_DIRECTORY) == 0)
		{
			write(2, "bash: cd: ", 10);
			write(2, arg, ft_strlen(arg));
			write(2, ": ", 2);
		}
		else
		{
			write(2, "bash: ", 6);
			write(2, arg, ft_strlen(arg));
			write(2, ": ", 2);
		}
	}
	write(2, msg, ft_strlen(msg));
	write(2, "\n", 1);
}
	*/
void	write_error_message(const char *msg, const char *arg)
{
	const char	*prefix;

	prefix = "bash: ";

	if (arg)
	{
		if (ft_strcmp(msg, MSG_ERR_CMD_NOT_FOUND) == 0)
			prefix = ""; // no "bash: ", solo "arg: msg"
		else if (ft_strcmp(msg, MSG_ERR_NOT_IS_DIRECTORY) == 0)
			prefix = "bash: cd: ";
		else if (ft_strcmp(msg, MSG_ERR_NOT_NUMERIC) == 0)
			prefix = "bash: exit: ";

		if (prefix[0])
			write(2, prefix, ft_strlen(prefix));
		write(2, arg, ft_strlen(arg));
		write(2, ": ", 2);
	}
	write(2, msg, ft_strlen(msg));
	write(2, "\n", 1);
}



void	ft_error(t_program *program, const char *msg, const char *arg, int exit_st)
{
	write_error_message(msg, arg);
	if (program)
		program->last_exit_status = exit_st;
}
