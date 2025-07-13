/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_fork_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzolotar <mzolotar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 15:07:56 by mzolotar          #+#    #+#             */
/*   Updated: 2025/07/12 12:18:33 by mzolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	wait_all_children(t_all *all, int *last_status, t_program *program)
{
	int	i;

	i = 0;
	while (i < (all->exec->num_pipes + 1))
	{
		waitpid(all->exec->pids[i], last_status, 0);
		if (WIFSIGNALED(*last_status))
		{
			if (WTERMSIG(*last_status) == SIGINT)
			{
				g_atomic = SIGINT;
				program->last_exit_status = 130;
			}
			else if (WTERMSIG(*last_status) == SIGQUIT)
			{
				g_atomic = SIGQUIT;
				program->last_exit_status = 131;
			}
		}
		else
			program->last_exit_status = WEXITSTATUS(*last_status);
		i++;
	}
}

void	handle_signal_output(t_program *program)
{
	(void)program;
	if (g_atomic == SIGQUIT)
		write(STDOUT_FILENO, "Quit (core dumped)\n", 18);
	if (g_atomic == SIGINT || g_atomic == SIGQUIT)
		write(STDOUT_FILENO, "\n", 2);
}

void	wait_child(t_all *all, t_program *program)
{
	int	status;

	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, handler);
	wait_all_children(all, &status, program);
	handle_signal_output(program);
	signal_handling();
	signal(SIGQUIT, SIG_IGN);
}
