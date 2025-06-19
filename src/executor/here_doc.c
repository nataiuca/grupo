/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzolotar <mzolotar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 13:07:50 by mzolotar          #+#    #+#             */
/*   Updated: 2025/06/11 12:37:31 by mzolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Opens a here-doc temporary file and sets its file descriptor.
 *
 * @param program Pointer to the main program structure.
 * @param here Pointer to the here-doc structure.
 * @param i Index of the here-doc to open.
 * @return 0 on success, -1 on failure.
 */
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

/**
 * @brief Checks if a line matches the here-doc delimiter.
 *
 * @param line Input line to compare.
 * @param delimiter The delimiter used to end the here-doc.
 * @return true if line is the delimiter, false otherwise.
 */
static bool	is_delimiter(char *line, char *delimiter)
{
	return (ft_strlen(line) == ft_strlen(delimiter) && ft_strncmp(line,
			delimiter, ft_strlen(delimiter)) == 0);
}

/**
 * @brief Processes lines for a here-doc until the delimiter is reached.
 *
 * @param all Pointer to the global structure.
 * @param program Pointer to the main program structure.
 * @param temp Token pointing to the here-doc redirection.
 * @param i Index for current here-doc file.
 * @return 0 on success, -1 on error.
 */
int	process_here_doc_lines(t_all *all, t_program *program, t_tokens *temp,
		int i)
{
	char		*here_line;
	t_tokens	*temp_line;

	temp_line = temp;
	here_line = get_line_str("> ", program);
	while (here_line)
	{
		if (is_delimiter(here_line, temp->next->content))
		{
			free(here_line);
			break ;
		}
		all->here->expanded_line_here = ft_strdup(here_line);
		if (!all->here->expanded_line_here)
			return (free(here_line), -1);
		if (temp_line->next->expand_here == true)
			check_and_expand_var(all->tokens, &all->here->expanded_line_here, all->meta,
				program);
		write_and_free_here_line(all->here->expanded_line_here,
			all->here->fd_array[i]);
		free(here_line);
		here_line = get_line_str("> ", program);
	}
	return (0);
}

/**
 * @brief Main loop to handle all here-doc entries in the token list.
 *
 * @param all Pointer to the global structure.
 * @param program Pointer to the main program structure.
 * @param here Pointer to the here-doc structure.
 * @return void
 */
void	here_doc(t_all *all, t_program *program, t_here *here)
{
	int			i;
	char		*itoa_str;
	t_tokens	*temp;

	temp = all->tokens;
	i = -1;
	while (++i < here->here_doc_num && all->tokens)
	{
		while (temp && (temp->type != redir_l_d || !temp->next
				|| temp->next->type != heredoc_delimiter))
		{
			temp = temp->next;
		}
		if (!temp || !temp->next)
			return ;
		itoa_str = ft_itoa(i);
		if (!itoa_str)
			return ;
		generate_here_doc_name(program, here, i, itoa_str);
		free(itoa_str);
		if (open_here_doc_file(program, here, i) < 0)
			return ;
		process_here_doc_lines(all, program, temp, i);
		temp = temp->next->next;
	}
}

/**
 * @brief Checks for here-docs and handles them if within allowed limit.
 *
 * @param all Pointer to the global structure.
 * @param program Pointer to the main program structure.
 * @param here Pointer to the here-doc structure.
 * @return void
 */
void	check_here_doc(t_all *all, t_program *program, t_here *here)
{
	count_here_doc(all->tokens, here);
	if (here->here_doc_num >= HERE_DOCS_MAX)
	{
		ft_error(program, ERR_HERE_DOC_SIZE, NULL, 1);
		return ;
	}
	here_doc(all, program, here);
}
