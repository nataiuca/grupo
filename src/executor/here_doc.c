/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzolotar <mzolotar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 13:07:50 by mzolotar          #+#    #+#             */
/*   Updated: 2025/06/23 09:11:49 by mzolotar         ###   ########.fr       */
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

/*
int	process_here_doc_lines(t_all *all, t_program *program, t_tokens *temp, int i)
{
	char		*here_line;
	t_tokens	*temp_line;
	//void		(*prev_handler)(int); // para restaurar después

	temp_line = temp;

	// 👉 Establecer handler personalizado para Ctrl+C en heredoc
	//prev_handler = signal(SIGINT, handler_herequote);
	signal(SIGINT, handler_herequote);
	here_line = herequote_hook_rl(program);
	while (here_line)
	{
		//signal(SIGINT, handler_herequote);
		if (herequote_check_g_atomic(program, here_line)) //se supone que aqui ya entra program->last_exit_status = 130;
		{
			//fprintf(stderr, "break con herequote_check_g_atomic\n ");
			break ;
		}
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
		here_line = herequote_hook_rl(program);
	}
	//program->last_exit_status = 130;
	return (0);
}
	*/
int	process_here_doc_lines(t_all *all, t_program *program, t_tokens *temp, int i)
{
	char		*here_line;
	t_tokens	*temp_line;
	bool		delimiter_reached = false;
	int			lineno = 1; // empieza en 1 si quieres simular línea 1 de heredoc

	temp_line = temp;
	signal(SIGINT, handler_herequote);
	here_line = herequote_hook_rl(program);

	while (here_line)
	{
		if (herequote_check_g_atomic(program, here_line))
			break ;

		if (is_delimiter(here_line, temp->next->content))
		{
			delimiter_reached = true;
			free(here_line);
			break ;
		}

		all->here->expanded_line_here = ft_strdup(here_line);
		if (!all->here->expanded_line_here)
			return (free(here_line), -1);

		if (temp_line->next->expand_here == true)
			check_and_expand_var(all->tokens, &all->here->expanded_line_here, all->meta, program);

		write_and_free_here_line(all->here->expanded_line_here, all->here->fd_array[i]);

		free(here_line);
		here_line = herequote_hook_rl(program);
		lineno++; // cuenta líneas para imprimir luego
	}

	// Si salimos por EOF sin haber alcanzado el delimitador
	if (!delimiter_reached && !g_atomic)
	{
		ft_putstr_fd("minishell: warning: here-document at line ", STDERR_FILENO);
		ft_putnbr_fd(lineno, STDERR_FILENO);
		ft_putstr_fd(" delimited by end-of-file (wanted `", STDERR_FILENO);
		ft_putstr_fd(temp->next->content, STDERR_FILENO);
		ft_putstr_fd("')\n", STDERR_FILENO);
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
	//fprintf(stderr, "salgo de here\n ");
}
