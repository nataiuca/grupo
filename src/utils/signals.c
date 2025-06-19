/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzolotar <mzolotar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 21:51:14 by mzolotar          #+#    #+#             */
/*   Updated: 2025/06/10 22:18:17 by mzolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void enable_echoctl(void)
{
	struct termios term;

	if (tcgetattr(STDIN_FILENO, &term) == -1)
	{
		perror("tcgetattr");
		exit(EXIT_FAILURE);
	}
	term.c_lflag |= ECHOCTL;  // <-- ACTIVA echo de caracteres de control
	if (tcsetattr(STDIN_FILENO, TCSANOW, &term) == -1)
	{
		perror("tcsetattr");
		exit(EXIT_FAILURE);
	}
}

void sigint_handler(int signal)
{
	(void)signal;
	write(STDOUT_FILENO, "^C\n", 3);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

/*
* catch control-c with handler and control-\ ignoring.
* SIGINT control-c
* SIGQUIT control-\
*/
void	catch_signal(void)
{
	signal (SIGINT, sigint_handler);
	signal (SIGQUIT, SIG_IGN);
}

/*
* catch control-d, get null input and print exit.
*/
void catch_interactive(t_program *program, char *input, char *prompt)
{
	(void)program;
    (void)prompt;
	if (!input)
	{
		printf("\033[1A");  // sube una línea
		printf("\r");       // cursor al inicio de la línea
		printf("\033[K");   // borra hasta final de línea
		printf("exit\n");
		free_program(program);
        exit(0);
	}
}

