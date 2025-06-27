
# include "minishell.h"

//-------------------DEBUG ---------------------//
void debug_open_fds()
{
    for (int fd = 0; fd < 1024; fd++)
    {
        if (fcntl(fd, F_GETFD) != -1)
            printf("FD %d is open\n", fd);
    }
}
/*
void	wait_for_children(t_all all, int num_cmds, t_program *program)
{
	int i;
	int status;
	pid_t waited;

	printf("⚠️DEBUG: entrando en wait_for_children\n");
	printf("⚠️DEBUG: num_cmds = %d\n", num_cmds);

	i = 0;
	while (i < num_cmds)
	{
		printf("⚠️DEBUG: esperando por PID #%d: %d\n", i, all.exec->pids[i]);

		if (all.exec->pids[i] > 0)
		{
			waited = waitpid(all.exec->pids[i], &status, 0);
			//waited = waitpid(-1, &status, 0);

			if (waited == -1)
				perror("waitpid");  // Te dice si hubo error en waitpid

			printf("⚠️DEBUG: waitpid retornó para PID %d con status raw: %d\n", waited, status);

			if (WIFEXITED(status))
			{
				int exit_status = WEXITSTATUS(status);
				printf("⚠️DEBUG: Proceso %d terminó normalmente con exit status: %d\n", waited, exit_status);
			}
			else if (WIFSIGNALED(status))
			{
				printf("⚠️DEBUG: Proceso %d fue terminado por señal: %d\n", waited, WTERMSIG(status));
			}
			else
			{
				printf("⚠️DEBUG: Proceso %d terminó de forma desconocida\n", waited);
			}
		}
		else
		{
			printf("⚠️DEBUG: PID #%d (%d) es inválido o no necesita esperar\n", i, all.exec->pids[i]);
		}

		i++;
	}

	program->last_exit_status = WEXITSTATUS(status);
	printf("⚠️DEBUG: saliendo de wait_for_children con último exit_status: %d\n", program->last_exit_status);
}
*/
//------------------ EJECUCIÓN -------------------//
bool must_use_env_builtin(char *cmd)
{
	if (!cmd)
		return (false);
	// Solo si el comando es exactamente "env"
	if (ft_strcmp(cmd, "env") == 0)
		return (true);
	return (false);
}

bool is_forbidden_env_path(char *cmd)
{
	if (!cmd)
		return false;

	return (
		ft_strcmp(cmd, "/usr/bin/env") == 0 ||
		ft_strcmp(cmd, "/bin/env") == 0 ||
		ft_strcmp(cmd, "./env") == 0 ||
		(ft_strcmp(cmd, "env") == 0 && !check_builtin(cmd))
	);
}

bool must_use_pwd_builtin(char *cmd)
{
	if (!cmd)
		return (false);
	// Solo si el comando es exactamente "env"
	if (ft_strcmp(cmd, "pwd") == 0)
		return (true);
	return (false);
}

bool is_forbidden_pwd_path(char *cmd)
{
	if (!cmd)
		return false;

	return (
		ft_strcmp(cmd, "/usr/bin/pwd") == 0 ||
		ft_strcmp(cmd, "/bin/pwd") == 0 ||
		ft_strcmp(cmd, "./pwd") == 0 ||
		(ft_strcmp(cmd, "pwd") == 0 && !check_builtin(cmd))
	);
}

static void	handle_fork_error(t_program *program, t_all *all)
{
	ft_error(program, MSG_ERR_FORK, NULL, 1);
	free_child(program, all);
	exit(1);
}

static void	exec_and_exit_child(t_all *all, t_tokens *curr, t_program *program, int i)
{
	int builtin_exit_code;

	if (all->exec->args && ft_strcmp(all->exec->args[0], ":") == 0)
	{
		if (!apply_redir(all, program)) // redirecciones como `> file`
		{
			free_child(program, all);
			exit(program->last_exit_status);
		}
		free_child(program, all);
		exit(0);
	}

	if (!apply_redir(all, program)|| !apply_cmd_redir(all->exec, i, program) )
	{
		free_child(program, all);
		exit(program->last_exit_status);
	}
	close_all_pipes(all->exec);

	if (is_forbidden_env_path(curr->content))
	{
		ft_error(program, "bash: env: external call not allowed", NULL, 127);
		free_child(program, all);
		exit(127);
	}
	if (is_forbidden_pwd_path(curr->content))
	{
		ft_error(program, "bash: pwd: external call not allowed", NULL, 127);
		free_child(program, all);
		exit(127);
	}

	if (check_builtin(curr->content) || must_use_env_builtin(curr->content) || must_use_pwd_builtin(curr->content))
	{
		//fprintf(stderr,"\033[0;34m ⚠️DEBUG: entra en builtins en hijo \n  \033[0m\n");
		builtin_exit_code = ft_builtin(all, program);
		free_child(program, all);
		exit(builtin_exit_code);
	}
	else 
	{
		//fprintf(stderr,"\033[0;34m ⚠️DEBUG: entra en exec_cmd en hijo \n  \033[0m\n");
		execute_command(all, program, curr);
	}
	
}


pid_t	fork_and_exec_command(t_all *all, t_tokens *curr, int i, t_program *program)
{
	pid_t pid;

	pid = fork();
	if (pid < 0)
		handle_fork_error(program, all);
	if (pid == 0)
	{
		signal(SIGINT, handler_child);
		signal(SIGQUIT, SIG_DFL);
		exec_and_exit_child(all, curr, program, i);
	}
	//fprintf(stderr,"\033[0;34m ⚠️DEBUG: p->l_e_s en fork_command: %d \n  \033[0m\n", program->last_exit_status);
	return (pid);
}

int	check_failed_redir_child(pid_t pid, t_program *program)
{
	int		status;
	pid_t	waited;

	waited = waitpid(pid, &status, WNOHANG);
	if (waited == -1)
	{
		ft_error(program, MSG_ERR_NO_SUCH_FILE, "waitpid", 1);
		return (1);
	}
	if (waited == 0)
		return (0); 

	if (WIFEXITED(status))
	{
		int exit_status = WEXITSTATUS(status);
		if (exit_status == 1)
		{
			ft_error(program, "Redirection failed in child process", NULL, 1);
			return (1);
		}
	}
	return (0);
}


void	wait_child(t_all *all, t_program *program)
{
	int		i;
	int		status;
	int		num_pids;
	int		atom=0;

	
	//no ignoras la señal en el padre mientras espera.
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, handler);
	num_pids = all->exec->num_pipes + 1;
	i = 0;
	while (i < num_pids)
	{
		waitpid(all->exec->pids[i], &status, 0);
		if (WIFSIGNALED(status))
		{
			if (WTERMSIG(status) == SIGINT)
				atom = SIGINT;
			else if (WTERMSIG(status) == SIGQUIT)
				atom = SIGQUIT;
		}
		i++;
	}
	// Obtener código de retorno del último hijo
	if (WIFEXITED(status))
	{
		program->last_exit_status = WEXITSTATUS(status);
		//fprintf(stderr, "DEBUG 1 : testeito en wait_child->exit_status: %d\n ", program->last_exit_status);
	}
	else if (WIFSIGNALED(status))
	{
		int signal_num = WTERMSIG(status);
		program->last_exit_status = 128 + signal_num;
		//fprintf(stderr, "DEBUG 2: testeito en wait_child->exit_status: %d\n ", program->last_exit_status);
		g_atomic = 128 + signal_num;
	}
	// Mensajes finales
	if (g_atomic == SIGQUIT)
		ft_error(program, "Quit (core dumped)", NULL, 131);
	if (g_atomic || atom)
	{
		//fprintf(stderr, "\nDEBUG 2: testeito en wait_child->g_atomic: %d\n ", g_atomic);
		//fprintf(stderr, "DEBUG 2: testeito en wait_child->atom: %d\n ", atom);
		write(STDOUT_FILENO, "\n", 1); //tu bien ->//modir, testeo
		//write(STDOUT_FILENO, "err2\n", 5); //tu bien ->//modir, testeo
	}
	signal(SIGINT, handler_builtins); // Restaurar señales
	signal(SIGQUIT, SIG_IGN);
}

void ft_exec(t_all *all, t_program *program)
{
	if (!all->tokens || !program)
		return;

	if (!update_args(all->exec, all->tokens, program))
		return;  // error actualizando args, salir

    if (all->exec->num_pipes == 0)
    {
        run_single_command(all, program);  // todavia no sabemos si es builting o execve, y no sabemos si por parde o hijo
		return;
    }
    else
    {
		if (executor_loop(all, program) == -1)
        	return; //⚠️return aplicado en apply_redirections
    }
	// ⚠️ Restaurar señales tras ejecutar comando (builtin o externo)
	signal_handling();
}
