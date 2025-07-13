/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzolotar <mzolotar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 14:38:35 by mzolotar          #+#    #+#             */
/*   Updated: 2025/07/06 09:12:20 by mzolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	count_here_doc(t_tokens *tokens, t_here *here)
{
	here->here_doc_num = 0;
	while (tokens)
	{
		if (tokens && tokens->next)
		{
			if (tokens->type == redir_l_d
				&& tokens->next->type == here_d)
			{
				here->here_doc_num++;
			}
		}
		tokens = tokens->next;
	}
	return ;
}

void	generate_here_doc_name(t_program *program, t_here *here, int i,
		char *itoa_str)
{
	if (here->here_name_docs[i])
	{
		free(here->here_name_docs[i]);
	}
	here->here_name_docs[i] = ft_strjoin3(here->base_cwd, "/heredoc_",
			itoa_str);
	if (!here->here_name_docs[i])
	{
		ft_error(program, MSG_ERR_MALLOC, NULL, 1);
	}
}

void	write_and_free_here_line(char *line, int fd)
{
	write(fd, line, ft_strlen(line));
	write(fd, "\n", 1);
	free(line);
}

bool	alloc_here_doc_names(t_here *here, t_program *program)
{
	int	i;

	i = 0;
	while (i < HERE_DOCS_MAX)
	{
		here->here_name_docs[i] = malloc(HOSTNAME_SIZE);
		if (!here->here_name_docs[i])
		{
			ft_error(program, MSG_ERR_MALLOC, NULL, 1);
			return (false);
		}
		here->here_name_docs[i][0] = '\0';
		here->fd_array[i] = -1;
		i++;
	}
	return (true);
}

void	free_here_doc_names(t_here *here, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		if (here->here_name_docs[i])
		{
			free(here->here_name_docs[i]);
		}
		i++;
	}
	free(here);
}
