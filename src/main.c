/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzolotar <mzolotar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 08:57:20 by mzolotar          #+#    #+#             */
/*   Updated: 2025/06/23 08:38:13 by mzolotar         ###   ########.fr       */
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
	rl_catch_signals = 0;
	signal_handling();
	minishell_loop(&program);
	//fprintf(stderr, "⛔ liberando prompt por main\n");
	free_program(&program);		//revisar este free
	return (0);
}
