/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natferna <natferna@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 14:38:35 by mzolotar          #+#    #+#             */
/*   Updated: 2025/06/26 21:43:50 by natferna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Counts the number of here-docs in the token list.
 *
 * @param tokens Pointer to the list of tokens to check.
 * @param here Pointer to the here-doc structure to update.
 * @return void
 */
void	count_here_doc(t_tokens *tokens, t_here *here)
{
	here->here_doc_num = 0;
	while (tokens)
	{
		if (tokens && tokens->next)
		{
			if (tokens->type == redir_l_d
				&& tokens->next->type == heredoc_delimiter)
			{
				here->here_doc_num++;
			}
		}
		tokens = tokens->next;
	}
	return ;
}

/**
 * @brief Generates a unique name for a here-doc temporary file.
 *
 * @param program Pointer to the main program structure.
 * @param here Pointer to the here-doc structure.
 * @param i Index for the current here-doc.
 * @param itoa_str Stringified index used in the file name.
 * @return void
 */
void generate_here_doc_name(t_program *program, t_here *here, int i, char *itoa_str)
{
    // Si ya existe un nombre (de una ejecución anterior o un error), liberarlo primero
    // Esto es importante si se llama varias veces para el mismo índice 'i'
    if (here->here_name_docs[i])
    {
        free(here->here_name_docs[i]);
        here->here_name_docs[i] = NULL;
    }

    // Asegurarse de que here->base_cwd no sea NULL antes de usarlo
    // Esto debería estar garantizado por init_exec_vals, pero es una buena defensa.
    if (!here->base_cwd)
    {
        ft_error(program, "Internal error: base_cwd is NULL for heredoc", NULL, 1);
        return; // No se puede generar el nombre
    }

    // ft_strjoin3 debe asignar una nueva cadena
    here->here_name_docs[i] = ft_strjoin3(here->base_cwd, "/heredoc_", itoa_str);
    if (!here->here_name_docs[i])
    {
        ft_error(program, MSG_ERR_MALLOC, NULL, 1);
    }
}

/**
 * @brief Writes a line to the here-doc file and frees the memory.
 *
 * @param line String to be written to the file.
 * @param fd File descriptor of the here-doc file.
 * @return void
 */
void	write_and_free_here_line(char *line, int fd)
{
	write(fd, line, ft_strlen(line));
	write(fd, "\n", 1);
	free(line);
}

/**
 * @brief Allocates memory for storing here-doc file names.
 *
 * @param here Pointer to the here-doc structure to initialize.
 * @param program Pointer to the main program structure.
 * @return true if allocation is successful, false otherwise.
 */
bool alloc_here_doc_names(t_here *here, t_program *program)
{
    int i;

    i = 0;
    while (i < HERE_DOCS_MAX)
    {
        here->here_name_docs[i] = NULL; // Inicializar a NULL, la asignación se hará en generate_here_doc_name
        here->fd_array[i] = -1;
        i++;
    }
    (void)program; // Para Norminette si 'program' no se usa aquí
    return (true);
}

/**
 * @brief Frees memory allocated for here-doc file names.
 *
 * @param here Pointer to the here-doc structure.
 * @param count Number of here-docs to free.
 * @return void
 */
void free_here_doc_names(t_here *here, int count)
{
    int i;

    i = 0;
    while (i < count)
    {
        if (here->here_name_docs[i]) // Solo libera si el puntero no es NULL
        {
            free(here->here_name_docs[i]);
            here->here_name_docs[i] = NULL; // Opcional, pero buena práctica
        }
        i++;
    }
    // No liberar 'here' aquí, ya que es el puntero a la estructura,
    // y se libera en free_here().
    // free(here); // <-- ¡NO DEBE ESTAR AQUÍ!
}
