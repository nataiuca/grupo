/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natferna <natferna@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 01:41:23 by natferna          #+#    #+#             */
/*   Updated: 2025/06/24 02:01:22 by natferna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Variable global definida en algún .c
extern volatile sig_atomic_t g_atomic;

// Padre (prompt interactivo): solo handle SIGINT
void parent_sigint_handler(int signo)
{
    if (signo == SIGINT)
    {
        g_atomic = 1;
        rl_replace_line("", 0);
        write(STDOUT_FILENO, "\n", 1);
        rl_on_new_line();
        rl_redisplay();
    }
}

// Hijo (tras fork, antes de execve): handle SIGINT y SIGQUIT
void child_signal_handler(int signo)
{
    if (signo == SIGINT)
        g_atomic = 130;
    else if (signo == SIGQUIT)
        g_atomic = 131;
}
void handler_herequote(int signum)
{
    if (signum == SIGINT)
    {
        g_atomic = 130;
        write(STDOUT_FILENO, "^C\n", 3);
        rl_done = 1;
    }
}

// Instala los handlers según el contexto:
//  parent = 1 → shell padre (prompt)
//  parent = 0 → procesos hijos
void setup_signals(int parent)
{
    struct sigaction sa;

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;

    if (parent)
        sa.sa_handler = parent_sigint_handler;
    else
        sa.sa_handler = child_signal_handler;

    sigaction(SIGINT, &sa, NULL);

    if (parent)
        // Padre ignora SIGQUIT (Ctrl-\)
        sigaction(SIGQUIT, &(struct sigaction){ .sa_handler = SIG_IGN }, NULL);
    else
        sigaction(SIGQUIT, &sa, NULL);
}

bool	catch_interactive(t_program *program, t_all *all, char *input)
{
	(void)program;		//❌ quitar
	(void)all;			//❌ quitar

	if (!input)
	{
		if (g_atomic == 2 || g_atomic == 0) // Ctrl+D (no error de signal)
		{
			write(STDOUT_FILENO, "exit1\n", 6); //modif, testeo
			g_atomic = 0;
			return true;
		}
		else if (g_atomic == 130)
		{
			write(STDOUT_FILENO, "exit2\n", 6); //modif, testeo
			g_atomic = 0;
			return true;
		}
		else // Ctrl+C u otro, no imprimir exit
		{
			write(STDOUT_FILENO, "exit3", 5); //modif, testeo
			g_atomic = 0;
			return true;
		}
	}
	return false;
}


