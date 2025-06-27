

#include "minishell.h"

//Gestiona señales en el proceso principal/padre (tu shell interactiva).
//🟢 Uso típico: Shell interactiva cuando estás esperando comandos del usuario.

void	handler(int signum)
{
	if (signum == SIGINT)
	{
		g_atomic = 130;
		write(STDOUT_FILENO, "^C\n", 3);
		rl_on_new_line();			//se posiciona en una nueva línea.
		rl_replace_line("", 0);		//borra la línea escrita.
		rl_redisplay();				// muestra un nuevo prompt limpio.
	}
	if (signum == SIGQUIT)
		ft_putstr("Quit (core dumped)\n");
}
		

//Asocia las señales con los manejadores adecuados al iniciar tu shell.
//🟢 Uso típico: Al comienzo de la shell para preparar las señales correctamente.
void	signal_handling(void)
{
	if (signal(SIGINT, handler) == SIG_ERR
		|| signal(SIGQUIT, SIG_IGN) == SIG_ERR)
	{
		perror("signal_handler: signal error\n");
		exit(EXIT_FAILURE);
	}
}
//Manejador de señales dentro del proceso hijo que ejecuta comandos.
//Uso típico: Se asigna en hijos después del fork() (antes del exec()).
void	handler_child(int signum)
{
	if (signum == SIGINT|| signum == SIGQUIT)
	{
		g_atomic = 130;
		write(STDOUT_FILENO, "\n", 1); //modir, testeo
		//write(STDOUT_FILENO, "err3\n", 5); //modir, testeo
		exit(0);
	}
}
//Controla interrupciones mientras se está leyendo un heredoc (<<).
//Uso típico: Cuando estás leyendo el contenido de un heredoc. Esto evita que el heredoc se quede colgado en Ctrl+C.
void	handler_herequote(int signum)
{
	if (signum == SIGINT)
	{
		g_atomic = 130;
		write(STDOUT_FILENO, "^C", 2);
		//write(STDOUT_FILENO, "^C2", 3);
		rl_done = 1;			//Setea rl_done = 1; → esto hace que readline finalice y devuelva.
	}
}
//Maneja señales durante la ejecución de builtins como cd, echo, export, etc., sin salir del proceso shell.
//Uso típico: Cuando ejecutas un builtin que no requiere fork (para que el prompt se limpie sin matar tu minishell).
void	handler_builtins(int signum)
{
	if (signum == SIGINT)
	{
		g_atomic = 130;
		write(STDOUT_FILENO, "\n", 1); //modir, testeo
		//write(STDOUT_FILENO, "err4\n", 5); //modir, testeo
	}
}
//CTRL + D
bool	catch_interactive(t_program *program, t_all *all, char *input)
{
	(void)program;		//❌ quitar
	(void)all;			//❌ quitar

	if (!input)
	{
		if (g_atomic == 2 || g_atomic == 0) // Ctrl+D (no error de signal)
		{
			write(STDOUT_FILENO, "exit\n", 5); //modif, testeo
			//write(STDOUT_FILENO, "exit1\n", 6); //modif, testeo
			g_atomic = 0;
			return true;
		}
		else if (g_atomic == 130)
		{
			write(STDOUT_FILENO, "exit\n", 5); //modif, testeo
			//write(STDOUT_FILENO, "exit2\n", 6); //modif, testeo
			g_atomic = 0;
			return true;
		}
		else // Ctrl+C u otro, no imprimir exit
		{
			write(STDOUT_FILENO, "exit", 4); //modif, testeo
			//write(STDOUT_FILENO, "exit3", 5); //modif, testeo
			g_atomic = 0;
			return true;
		}
	}
	return false;
}


