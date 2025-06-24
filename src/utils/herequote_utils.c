
#include "minishell.h"

#include <signal.h>

/*
 * hook para leer el heredoc, usando sigaction() en lugar de signal()
 */
char *herequote_hook_rl(t_program *program)
{
    char                *line;
    struct sigaction    sa, old_sa;

    (void)program;

    /* Instalamos handler de SIGINT solo para heredoc */
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;           /* read‐line se reinicia */
    sa.sa_handler = handler_herequote;
    sigaction(SIGINT, &sa, &old_sa);

    /* Callback de readline para comprobar g_atomic */
    rl_event_hook = event_hook;
    line = readline("> ");

    /* Restauramos el handler anterior de SIGINT */
    rl_event_hook = NULL;
    sigaction(SIGINT, &old_sa, NULL);

    return line;
}

int	herequote_check_g_atomic(t_program *program, char *here_line)
{
	if (g_atomic)
	{
		if (here_line)
			free(here_line);
		rl_event_hook = NULL;
		program->last_exit_status = 130;
		//fprintf(stderr, "program->last_exit_status en herequote_check_g_atomic: %d\n ", program->last_exit_status);
		return (1);
	}
	return (0);
}

int	event_hook(void)
{
	if (g_atomic)
	{
		rl_done = 1;
		return (1);
	}
	return (0);
}