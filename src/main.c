/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natferna <natferna@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 08:57:20 by mzolotar          #+#    #+#             */
/*   Updated: 2025/06/24 01:55:28 by natferna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_atomic = 0;

int main(int argc, char **argv, char **envp)
{
    t_program program;

    if (argc != 1)
    {
        printf("Usage: %s\n", argv[0]);
        return EXIT_FAILURE;
    }

    init_program(&program, envp);

    program.is_interactive = isatty(STDIN_FILENO);

    /* readline no instala manejadores propios */
    rl_catch_signals = 0;

    if (program.is_interactive)
    {
        setup_signals(1);  /* Padre: modo interactivo */
    }

    minishell_loop(&program);

    free_program(&program);
    return program.last_exit_status;
}
