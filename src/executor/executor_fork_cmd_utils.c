/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_fork_cmd_utils.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzolotar <mzolotar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 07:50:09 by mzolotar          #+#    #+#             */
/*   Updated: 2025/06/30 08:22:13 by mzolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "minishell.h"

int	init_executor(t_program *program, t_all *all)
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

int	handle_fork_error(t_program *program, t_all *all, t_tokens *original, int i)
{
	ft_error(program, MSG_ERR_FORK, NULL, 1);
	close_all_pipe_fds_in_parent(all->exec);
	if (all->exec->pipe_fds[i][0] != -1)
		free_all_pipes(all->exec);
	all->tokens = original;
	return (-1);
}

void	advance_to_next_command(t_tokens **curr)
{
	while (*curr && (*curr)->type != pipe_type)
		*curr = (*curr)->next;
	if (*curr)
		*curr = (*curr)->next;
}

void	finalize_executor(t_all *all, t_program *program)
{
	close_all_pipe_fds_in_parent(all->exec);
	wait_child(all, program);
	free_all_pipes(all->exec);
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
