/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natferna <natferna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 08:57:20 by mzolotar          #+#    #+#             */
/*   Updated: 2025/06/23 20:39:38 by natferna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_atomic = 0;

int main (int argc, char **argv, char **envp)
{
	t_program program;

	(void)argv;
	if (argc != 1)
	{
		printf("Usage: ./minishell\n");
		return (1);
	}
	init_program(&program, envp);
	program.is_interactive = isatty(STDIN_FILENO);
	rl_catch_signals = 0;
	signal_handling();
	minishell_loop(&program);
	//fprintf(stderr, "⛔ liberando prompt por main\n");
	free_program(&program);		//revisar este free
	return (0);
}
