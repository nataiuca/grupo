/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natferna <natferna@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 12:01:25 by mzolotar          #+#    #+#             */
/*   Updated: 2025/07/03 21:18:24 by natferna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	process_output_redirection(t_program *program, t_exec *exec, t_tokens *curr)
{
	int	flags;
	int	result;

	flags = get_output_flags(curr->type);
	if (!curr->next || !curr->next->content || curr->next->content[0] == '\0')
	{
		ft_error(program, "Missing redirection target", NULL, 1);
		return (-1);
	}
	result = handle_output(program, exec, curr->next->content, flags);
	if (result == -1 || exec->outfile == -1)
		return (-1);
	return (0);
}

int	open_heredoc_file(t_all *all, t_program *program, t_tokens *curr,
		int *heredoc_index)
{
	char	*file;

	(void)curr;
	file = all->here->here_name_docs[*heredoc_index];
	if (!file)
		return (ft_error(program, "Missing heredoc file", NULL, 1), -1);
	if (all->exec->infile > 2)
		close(all->exec->infile);
	all->exec->infile = open(file, O_RDONLY);
	if (all->exec->infile < 0)
		return (ft_error(program, strerror(errno), file, 1), -1);
	(*heredoc_index)++;
	return (0);
}

int	open_input_file(t_all *all, t_program *program, t_tokens *curr)
{
	if (!curr->next || !curr->next->content || curr->next->content[0] == '\0')
		return (ft_error(program, "Missing redirection target", NULL, 1), -1);
	if (handle_input(program, all->exec, curr->next->content) < 0
		|| all->exec->infile == -1)
		return (-1);
	return (0);
}

int	process_input_redirection(t_all *all, t_program *program, t_tokens *curr,
		int *heredoc_index)
{
	if (curr->type == redir_l_d)
		return (open_heredoc_file(all, program, curr, heredoc_index));
	return (open_input_file(all, program, curr));
}

bool	apply_redir(t_all *all, t_program *program)
{
	t_tokens	*curr;
	int			heredoc_index;
	int			ret;

	curr = all->tokens;
	heredoc_index = 0;
	while (curr && curr->type != pipe_type)
	{
		if (curr->type == redir_r_u || curr->type == redir_r_d)
		{
			ret = process_output_redirection(program, all->exec, curr);
			if (ret == -1 || all->exec->outfile == -1)
				return (false);
		}
		else if (curr->type == redir_l_u || curr->type == redir_l_d)
		{
			ret = process_input_redirection(all, program, curr, &heredoc_index);
			if (ret == -1 || all->exec->infile == -1)
				return (false);
		}
		curr = curr->next;
	}
	return (true);
}
