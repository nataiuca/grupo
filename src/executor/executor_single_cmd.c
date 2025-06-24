

#include "minishell.h"

int check_child(t_program *program, t_all *all)
{
	if (all->exec->args && ft_strcmp(all->exec->args[0], ":") == 0)
	{
		if (!apply_redir(all, program)) // redirecciones como `> file`
			return (1);
		return (2);
	}
	if (!apply_redir(all, program) || !config_fd_redir(all->exec->infile, all->exec->outfile, program))
		return (1);
	return (0);
}

/* executor.c */

void fork_and_run_single_cmd(t_all *all, t_program *program, t_tokens *curr)
{
    pid_t pid;
    int   status;
    int   check_child_result;

    pid = fork();
    if (pid < 0)
    {
        ft_error(program, MSG_ERR_FORK, NULL, 1);
        return;
    }
    if (pid == 0)
    {
        /* — Contexto HIJO — */
        setup_signals(0);  /* child: SIGINT→130, SIGQUIT→131 */
        check_child_result = check_child(program, all);
        if (check_child_result == 1)
            free_and_exit_child(all, program, program->last_exit_status);
        if (check_child_result == 2)
            free_and_exit_child(all, program, 0);
        execute_command(all, program, curr);
        exit(EXIT_FAILURE);
    }
    else
    {
        /* — Contexto PADRE — */
        waitpid(pid, &status, 0);

        setup_signals(1);  /* padre: Ctrl-C limpia línea, Ctrl-\ ignorado */

        if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
        {
            write(STDOUT_FILENO, "^C\n", 3);
            program->last_exit_status = 130;
        }
        else
        {
            program->last_exit_status = WEXITSTATUS(status);
        }
    }
}

/* builtins_or_pipe.c */
void run_single_command(t_all *all, t_program *program)
{
    t_tokens *first_cmd = find_cmd(all->tokens);

    /* ... casos de forbidden/env/pwd ... */

    if (first_cmd && check_builtin(first_cmd->content))
    {
        if (!apply_redir(all, program))
            return;

        /* Mismo comportamiento que en el prompt */
        setup_signals(1);

        ft_builtin(all, program);

        /* Y lo restauramos otra vez */
        setup_signals(1);
        return;
    }

    fork_and_run_single_cmd(all, program, first_cmd);
}
