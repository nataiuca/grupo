/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzolotar <mzolotar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 13:07:50 by mzolotar          #+#    #+#             */
/*   Updated: 2025/07/01 13:34:11 by mzolotar         ###   ########.fr       */
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


static bool	is_delimiter(char *line, char *delimiter)
{
	return (ft_strlen(line) == ft_strlen(delimiter) && ft_strncmp(line,
			delimiter, ft_strlen(delimiter)) == 0);
}

int	process_here_doc_lines(t_all *all, t_program *program, t_tokens *temp, int i)
{
	char		*here_line;
	t_tokens	*temp_line;
	bool		delimiter_reached = false;
	int			lineno = 1; // empieza en 1 si quieres simular línea 1 de heredoc

	temp_line = temp;
	signal(SIGINT, handler_herequote);
	here_line = herequote_hook_rl(program);
	if (herequote_check_g_atomic(program, here_line))
	{
		signal(SIGINT, handler); // ⬅ restaurar antes de salir
		return (HEREDOC_SIGNAL); // ⚠️ AVISA a la capa superior que hubo cancelación
	}
	while (here_line)
	{
		if (herequote_check_g_atomic(program, here_line))
		{
			signal(SIGINT, handler); // ⬅ restaurar antes de salir
			return (HEREDOC_SIGNAL);
		}
		if (is_delimiter(here_line, temp->next->content))
		{
			delimiter_reached = true;
			free(here_line);
			signal(SIGINT, handler); // ⬅ restaurar antes de salir
			break ;
		}
		all->here->expanded_line_here = ft_strdup(here_line);
		if (!all->here->expanded_line_here)
		{
			ft_error( program, "memory allocation error in heredoc", NULL, 1);
			signal(SIGINT, handler); // ⬅ restaurar antes de salir
			return (free(here_line), HEREDOC_ALLOC_FAIL);
		}
		if (temp_line->next->expand_here == true)
			check_and_expand_var(all->tokens, &all->here->expanded_line_here, all->meta, program);

		write_and_free_here_line(all->here->expanded_line_here, all->here->fd_array[i]);
		free(here_line);
		here_line = herequote_hook_rl(program);
		lineno++; // cuenta líneas para imprimir luego
	}
	if (!delimiter_reached) 
	{
		signal(SIGINT, handler); 
		if (!g_atomic)
		{
			// Warning visible para usuario (Ctrl+D sin delimitador)
			ft_putstr_fd("minishell: warning: here-document at line ", STDERR_FILENO);
			ft_putnbr_fd(lineno, STDERR_FILENO);
			ft_putstr_fd(" delimited by end-of-file (wanted `", STDERR_FILENO);
			ft_putstr_fd(temp->next->content, STDERR_FILENO);
			ft_putstr_fd("')\n", STDERR_FILENO);
			return HEREDOC_OK;
		}
		else
			return HEREDOC_SIGNAL; // Ctrl+C interrumpió heredoc
	}
	signal(SIGINT, handler); 
	return HEREDOC_OK;
}


bool here_doc(t_all *all, t_program *program, t_here *here, t_tokens *tokens)
{
	int			i;
	char		*itoa_str;
	t_tokens	*temp;
	int			ret;

	temp = tokens;
	i = here->here_doc_created; // ⚠️ nuevo campo: cantidad de heredocs ya procesados
	while (temp)
	{
		if (temp->type == redir_l_d && temp->next && temp->next->type == heredoc_delimiter)
		{
			itoa_str = ft_itoa(i);
			if (!itoa_str)
				return false;
			generate_here_doc_name(program, here, i, itoa_str);
			free(itoa_str);
			if (open_here_doc_file(program, here, i) < 0)
			{
				signal(SIGINT, handler); // ✅ restaurar antes de salir del error
				return false;
			}
			ret = process_here_doc_lines(all, program, temp, i);
			if (ret != HEREDOC_OK) // Gestión centralizada de errores
			{
				if (all->here->fd_array[i] != -1)
				{
					close(all->here->fd_array[i]);
					all->here->fd_array[i] = -1;
				}
				here->here_doc_created = 0;
				return false;
			}
			// después de procesar todos los heredocs
			close(all->here->fd_array[i]);
			all->here->fd_array[i] = -1;
			i++;
		}
		temp = temp->next;
	}
	here->here_doc_created = i; // actualizar cuántos heredocs se han creado
	return true;
}


bool	check_here_doc(t_all *all, t_program *program, t_here *here)
{
	count_here_doc(all->tokens, here);
	if (here->here_doc_num >= HERE_DOCS_MAX)
	{
		ft_error(program, ERR_HERE_DOC_SIZE, NULL, 1);
		return (false);
	}
	here->here_doc_created = 0; // Reinicia el contador antes de crear

	// Procesa todos los heredocs linealmente
	if (!here_doc(all, program, here, all->tokens))
		return (false);
	return (true);
}
	


