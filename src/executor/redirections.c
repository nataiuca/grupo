/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzolotar <mzolotar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 12:01:30 by mzolotar          #+#    #+#             */
/*   Updated: 2025/07/01 13:12:38 by mzolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "minishell.h"

int get_output_flags(int type)
{
	if (type == redir_r_u)
		return (O_WRONLY | O_CREAT | O_TRUNC);
	return (O_WRONLY | O_CREAT | O_APPEND);
}

bool	config_fd_redir(int infile, int outfile, t_program *program)
{
	if (infile > 0 && infile != STDIN_FILENO)
	{
		if (dup2(infile, STDIN_FILENO) == -1)
		{
			//fprintf(stderr,"\033[0;34m ⚠️ DEBUG: infile EN  setup_fd_redirects  \033[0m\n");
			ft_error(program, strerror(errno), "dup2 infile", 1);
			return (false);
		}
		close(infile);
	}
	if (outfile != -1 && outfile != STDOUT_FILENO)
	{
		if (dup2(outfile, STDOUT_FILENO) == -1)
		{
			//fprintf(stderr,"\033[0;34m ⚠️ DEBUG: outfile EN  setup_fd_redirects  \033[0m\n");
			ft_error(program, strerror(errno), "dup2 outfile", 1);
			return (false);
		}
		close(outfile);
	}
	return (true);
}

static bool	redirect_input(t_exec *exec, int i, t_program *program)
{
	if (exec->infile != -1)
	{
		//fprintf(stderr, "⚠️DEBUG -> REDIR: dup2 infile %d -> stdin\n", exec->infile);
		if (dup2(exec->infile, STDIN_FILENO) == -1)
			return (ft_error(program, strerror(errno), "dup2 infile", 1), false);
	}
	else if (i > 0)
	{
		//fprintf(stderr, "⚠️DEBUG -> REDIR: dup2 pipe[%d][0] (%d) -> stdin\n", i - 1, exec->pipe_fds[i - 1][0]);
		if (dup2(exec->pipe_fds[i - 1][0], STDIN_FILENO) == -1)
			return (ft_error(program, strerror(errno), "dup2 pipe infile", 1), false);
	}
	return (true);
}

 bool	redirect_output(t_exec *exec, int i, t_program *program)
{
	if (exec->outfile != -1)
	{
		//fprintf(stderr, "⚠️DEBUG -> REDIR: dup2 outfile %d -> stdout\n", exec->outfile);
		//fprintf(stderr, "arg[0]: %s\n", exec->args ? exec->args[0] : "(null)");
		//fprintf(stderr, "  -> infile: %d, outfile: %d\n", exec->infile, exec->outfile);		
		if (dup2(exec->outfile, STDOUT_FILENO) == -1)
			return (ft_error(program, strerror(errno), "dup2 outfile", 1), false);
	}
	else if (i < exec->num_pipes)
	{
		//fprintf(stderr, "⚠️DEBUG -> REDIR: dup2 pipe[%d][1] (%d) -> stdout\n", i, exec->pipe_fds[i][1]);
		if (dup2(exec->pipe_fds[i][1], STDOUT_FILENO) == -1)
			return (ft_error(program, strerror(errno), "dup2 pipe outfile", 1), false);
	}
	return (true);
}

bool	apply_cmd_redir(t_exec *exec, int i, t_program *program)
{
	int	j;

	j = 0;
	if (!redirect_input(exec, i, program))
		return (false);
	if (!redirect_output(exec, i, program))
		return (false);
	while (j < exec->num_pipes)
	{
		close(exec->pipe_fds[j][0]);
		close(exec->pipe_fds[j][1]);
		j++;
	}
	return (true);
}
