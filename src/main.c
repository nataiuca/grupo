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

int main(int argc, char **argv, char **envp)
{
	t_program program;

	(void)argv;
	if (argc != 1)
	{
		printf("Usage: ./minishell\n");
		return (1);
	}
	init_program(&program, envp);

	// 🔧 Desactivar las señales automáticas de readline
	rl_catch_signals = 0;

	// 🚦 Configura señales generales (si tienes señales globales, por ejemplo, durante inicialización)
	catch_signal(); // solo si quieres que esté activa desde el arranque

	minishell_loop(&program); // loop principal

	free_program(&program);
	return (0);
}
