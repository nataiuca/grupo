/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzolotar <mzolotar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 08:57:20 by mzolotar          #+#    #+#             */
/*   Updated: 2025/06/12 11:56:38 by mzolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//int	g_signal = 0;

int main (int argc, char **argv, char **envp)
{
	t_program program;

	(void)argv;
	//(void)argc;
	if (argc != 1)
	{
		printf("Usage: ./minishell\n");
		return (1);
	}
	init_program(&program, envp);
	
	//rl_catch_signals = 0;
	//set_signals(program);
	
	minishell_loop(&program);		 //main loop
	
	//printf("\033[0;31m ⚠️DEBUG: volvemos a main y liberamos program\n \033[0m\n"); 			//⚠️testeo
	free_program(&program);
	return (0);
}
	