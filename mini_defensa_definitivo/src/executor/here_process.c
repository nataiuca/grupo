/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_process.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzolotar <mzolotar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 16:37:56 by natferna          #+#    #+#             */
/*   Updated: 2025/07/06 09:57:59 by mzolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_here_loop_state(char **line, bool *reached, int *lineno,
		t_program *prog)
{
	signal(SIGINT, handler_herequote);
	*line = herequote_hook_rl(prog);
	*reached = false;
	*lineno = 1;
}

int	check_signal_and_line(t_program *program, char *line)
{
	if (herequote_check_g_atomic(program, line))
	{
		signal(SIGINT, handler);
		return (1);
	}
	return (0);
}

int	process_delimiter(char *line, char *delim, bool *reached)
{
	if (ft_strlen(line) == ft_strlen(delim) && ft_strncmp(line, delim,
			ft_strlen(delim)) == 0)
	{
		*reached = true;
		free(line);
		signal(SIGINT, handler);
		return (1);
	}
	return (0);
}

int	process_and_write_line(t_all *all, t_program *program, t_tokens *temp,
		int i)
{
	all->here->expanded_line_here = ft_strdup(all->here->here_line);
	if (!all->here->expanded_line_here)
	{
		ft_error(program, "memory allocation error in heredoc", NULL, 1);
		signal(SIGINT, handler);
		free(all->here->here_line);
		return (0);
	}
	if (temp->next->expand_here == true)
		check_and_expand_var(all->tokens, &all->here->expanded_line_here,
			all->meta, program);
	write_and_free_here_line(all->here->expanded_line_here,
		all->here->fd_array[i]);
	free(all->here->here_line);
	return (1);
}

int	finalize_here_loop(char *delim, bool reached, int lineno)
{
	signal(SIGINT, handler);
	if (!reached)
	{
		if (!g_atomic)
		{
			ft_putstr_fd("minishell: warning: here-document at line ",
				STDERR_FILENO);
			ft_putnbr_fd(lineno, STDERR_FILENO);
			ft_putstr_fd(" delimited by end-of-file (wanted `", STDERR_FILENO);
			ft_putstr_fd(delim, STDERR_FILENO);
			ft_putstr_fd("')\n", STDERR_FILENO);
			return (HEREDOC_OK);
		}
		return (HEREDOC_SIGNAL);
	}
	return (HEREDOC_OK);
}
