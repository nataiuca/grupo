/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzolotar <mzolotar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 13:07:50 by mzolotar          #+#    #+#             */
/*   Updated: 2025/07/07 13:55:27 by mzolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	open_here_doc_file(t_program *program, t_here *here, int i)
{
	here->fd_array[i] = open(here->here_name_docs[i],
			O_RDWR | O_CREAT | O_TRUNC, 0666);
	if (here->fd_array[i] < 0)
	{
		ft_error(program, "Error opening here-doc file", NULL, 1);
		return (-1);
	}
	return (0);
}

void	close_fd(t_here *here, int i)
{
	if (here->fd_array[i] != -1)
	{
		close(here->fd_array[i]);
		here->fd_array[i] = -1;
	}
}

int	process_here_lines(t_all *all, t_program *program, t_tokens *temp, int i)
{
	bool	delimiter_reached;
	int		lineno;

	init_here_loop_state(&all->here->here_line, &delimiter_reached, &lineno,
		program);
	if (check_signal_and_line(program, all->here->here_line))
		return (HEREDOC_SIGNAL);
	while (all->here->here_line)
	{
		if (check_signal_and_line(program, all->here->here_line))
			return (HEREDOC_SIGNAL);
		if (process_delimiter(all->here->here_line, temp->next->content,
				&delimiter_reached))
			break ;
		if (!process_and_write_line(all, program, temp, i))
			return (HEREDOC_ALLOC_FAIL);
		all->here->here_line = herequote_hook_rl(program);
		lineno++;
	}
	return (finalize_here_loop(temp->next->content, delimiter_reached,
			lineno));
}

bool	here_doc(t_all *all, t_program *program, t_here *here, t_tokens *tokens)
{
	char		*itoa_str;
	t_tokens	*temp;
	int			ret;

	temp = tokens;
	while (temp)
	{
		if (temp->type == redir_l_d && temp->next && temp->next->type == here_d)
		{
			itoa_str = ft_itoa(here->here_c);
			if (!itoa_str)
				return (false);
			generate_here_doc_name(program, here, here->here_c, itoa_str);
			free(itoa_str);
			if (open_here_doc_file(program, here, here->here_c) < 0)
				return (signal(SIGINT, handler), false);
			ret = process_here_lines(all, program, temp, here->here_c);
			if (ret != HEREDOC_OK)
				return (close_fd(here, here->here_c), here->here_c = 0, false);
			close_fd(here, here->here_c);
			here->here_c++;
		}
		temp = temp->next;
	}
	return (true);
}

bool	check_here_doc(t_all *all, t_program *program, t_here *here)
{
	count_here_doc(all->tokens, here);
	if (here->here_doc_num >= HERE_DOCS_MAX)
	{
		ft_error(program, ERR_HERE_DOC_SIZE, NULL, 1);
		return (false);
	}
	here->here_c = 0;
	if (!here_doc(all, program, here, all->tokens))
		return (false);
	return (true);
}
