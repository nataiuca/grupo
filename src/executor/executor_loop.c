

# include "minishell.h"

static int	init_executor(t_program *program, t_all *all)
{
	int num_pids;
	
	if (all->exec->num_pipes == 1)
		num_pids = 2;
	else 
		num_pids = all->exec->num_pipes + 1;
	if (!init_all_pipes(program, all))
	{
		ft_error(program, MSG_ERR_PIPE, NULL, 1);
		return (-1);
	}
	all->exec->pids = malloc(sizeof(pid_t) * num_pids);
	if (!all->exec->pids)
	{
		ft_error(program, MSG_ERR_MALLOC, NULL, 1);
		free_child(program, all);
		return (-1);
	}
	return (0);
}

static int	handle_fork_error(t_program *program, t_all *all, t_tokens *original, int i)
{
	ft_error(program, MSG_ERR_FORK, NULL, 1);
	close_all_pipe_fds_in_parent(all->exec);
	if (all->exec->pipe_fds[i][0] != -1)
		free_all_pipes(all->exec);
	all->tokens = original;
	return (-1);
}

static int	should_skip_command(t_tokens **curr, t_tokens **cmd_token)
{
	*cmd_token = skip_redir(*curr);
	if (!*cmd_token)
	{
		*curr = (*curr)->next;
		return (1); // indicar que se debe saltar este comando
	}
	return (0);
}

static int	prepare_and_fork_command(t_program *program, t_all *all, t_tokens *cmd_token, int i)
{
	t_all *local_all;

	if (!update_args(all->exec, cmd_token, program))
		return (handle_fork_error(program, all, all->tokens_head, i));
	local_all = all;
	local_all->tokens = all->tokens;
	all->exec->pids[i] = fork_and_exec_command(local_all, cmd_token, i, program);
	if (all->exec->pids[i] == -1
		|| check_failed_redir_child(all->exec->pids[i], program))
		return (handle_fork_error(program, all, all->tokens_head, i));
	close_unused_pipes_in_parent(all->exec, i);
	return (0);
}

static void	advance_to_next_command(t_tokens **curr)
{
	while (*curr && (*curr)->type != pipe_type)
		*curr = (*curr)->next;
	if (*curr)
		*curr = (*curr)->next;
}

int	fork_command_loop(t_program *program, t_all *all)
{
	t_tokens	*curr;
	t_tokens	*cmd_token;
	int			i;

	curr = all->tokens;
	i = 0;
	while (curr)
	{
		if (should_skip_command(&curr, &cmd_token))
			continue;
		all->tokens = curr;
		if (prepare_and_fork_command(program, all, cmd_token, i))
			return (-1);
		i++;
		advance_to_next_command(&curr);
	}
	return (0);
}

static void	finalize_executor(t_all *all, t_program *program)
{
	close_all_pipe_fds_in_parent(all->exec);
	wait_child(all, program);
	free_all_pipes(all->exec);
}

int	executor_loop(t_all *all, t_program *program)
{
	all->tokens_head = all->tokens;
	if (init_executor(program, all) == -1)
		return (-1);
	if (fork_command_loop(program, all) == -1)
		return (-1);
	all->tokens = all->tokens_head;
	finalize_executor(all, program);
	return (0);
}
