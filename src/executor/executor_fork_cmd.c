/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_fork_cmd.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzolotar <mzolotar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 07:51:22 by mzolotar          #+#    #+#             */
/*   Updated: 2025/07/01 11:58:31 by mzolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "minishell.h"

pid_t	fork_and_exec_command(t_all *all, t_tokens *curr, int i, t_program *program)
{
	pid_t pid;

	pid = fork();
	if (pid < 0)
		handle_fork_error_2(program, all);
	if (pid == 0)
	{
		signal(SIGINT, handler_child);
		signal(SIGQUIT, SIG_DFL);
		exec_and_exit_child(all, curr, program, i);
	}
	return (pid);
}

int prepare_and_fork_command(t_program *program, t_all *all, t_tokens *cmd_token, int i)
{
	//t_all *local_all;

	if (!update_args(all->exec, cmd_token, program))
		return (handle_fork_error(program, all, all->tokens_head, i));
	//local_all = all;
	//local_all->tokens = all->tokens;
	all->exec->pids[i] = fork_and_exec_command(all, cmd_token, i, program);
	if (all->exec->pids[i] == -1
		|| check_failed_redir_child(all->exec->pids[i], program))
		return (handle_fork_error(program, all, all->tokens_head, i));
	close_unused_pipes_in_parent(all->exec, i);
	return (0);
}

int	fork_command_loop(t_program *program, t_all *all)
{
	t_tokens	*cmd_start;
	t_tokens	*curr;
	t_tokens	*cmd_token;
	int			i;

	curr = all->tokens;
	i = 0;
	while (curr)
	{
		cmd_start = curr;
		cmd_token = skip_redir(curr);
		if (!cmd_token) 
		{
			curr = curr->next;
			continue;
		}
		all->tokens = cmd_start;  // 🔧 Pasas desde el token que incluye redirs
		if (prepare_and_fork_command(program, all, cmd_token, i))
			return (-1);
		i++;
		advance_to_next_command(&curr);
	}
	return (0);
}

void	wait_child(t_all *all, t_program *program) // 🚩 NORMA
{
	int		i;
	int		status;
	int		num_pids;
	int		atom;

	atom=0;
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
	if (WIFEXITED(status))				// Obtener código de retorno del último hijo
		program->last_exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		int signal_num = WTERMSIG(status);
		program->last_exit_status = 128 + signal_num;
		g_atomic = 128 + signal_num;
	}
	if (g_atomic == SIGQUIT)			// Mensajes finales
		ft_error(program, "Quit (core dumped)", NULL, 131);
	if (g_atomic || atom)
		write(STDOUT_FILENO, "\n", 1); //tu bien ->//modir, testeo
	signal(SIGINT, handler_builtins); // Restaurar señales
	signal(SIGQUIT, SIG_IGN);
}

int	executor_loop(t_all *all, t_program *program)
{
	all->tokens_head = all->tokens;
	if (init_executor(program, all) == -1)
		return (-1);
		
//------------DEBUG--------------------------------------------
//	for (int i = 0; i < all->here->here_doc_created; i++)
//		fprintf(stderr, "✅ HEREDOC [%d]: %s\n", i, all->here->here_name_docs[i]);

//--------------------------------------------
	if (fork_command_loop(program, all) == -1)
		return (-1);
	all->tokens = all->tokens_head;
	finalize_executor(all, program);
	return (0);
}
