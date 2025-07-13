/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzolotar <mzolotar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 08:50:51 by mzolotar          #+#    #+#             */
/*   Updated: 2025/07/12 11:19:01 by mzolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handler_builtins(int signum)
{
	if (signum == SIGINT)
	{
		g_atomic = 2;
		write(STDOUT_FILENO, "\n", 1);
	}
}

bool	catch_ctrl_d(t_program *program, t_all *all, char *input)
{
	(void)program;
	(void)all;
	if (!input)
	{
		if (g_atomic == 2 || g_atomic == 0)
		{
			write(STDOUT_FILENO, "exit\n", 5);
			g_atomic = 0;
			return (true);
		}
		else if (g_atomic == 3)
		{
			write(STDOUT_FILENO, "exit\n", 5);
			g_atomic = 0;
			return (true);
		}
		else
		{
			write(STDOUT_FILENO, "exit", 4);
			g_atomic = 0;
			return (true);
		}
	}
	return (false);
}
