/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzolotar <mzolotar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 08:51:11 by mzolotar          #+#    #+#             */
/*   Updated: 2025/07/12 11:53:20 by mzolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handler(int signum)
{
	if (signum == SIGINT)
	{
		g_atomic = signum;
		write(STDOUT_FILENO, "^C\n", 3);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	signal_handling(void)
{
	if (signal(SIGINT, handler) == SIG_ERR || signal(SIGQUIT,
			SIG_IGN) == SIG_ERR)
	{
		perror("signal_handler: signal error\n");
		exit(EXIT_FAILURE);
	}
}

void	handler_child(int signum)
{
	if (signum == SIGINT || signum == SIGQUIT)
	{
		g_atomic = signum;
		write(STDOUT_FILENO, "\n", 1);
		exit(0);
	}
}

void	handler_herequote(int signum)
{
	if (signum == SIGINT)
	{
		g_atomic = signum;
		write(STDOUT_FILENO, "^C", 2);
		rl_done = 1;
	}
}
