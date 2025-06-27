/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_error_token.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natferna <natferna@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 11:22:55 by mzolotar          #+#    #+#             */
/*   Updated: 2025/06/27 01:58:36 by natferna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Checks if a pipe appears in a syntactically invalid position.
 *
 * @param tokens Pointer to the token list.
 * @return true if pipe misuse is found, false otherwise.
 */
bool	check_bash_pipe_before_here(t_tokens *tokens)
{
	t_tokens	*prev_token;

	if (tokens && tokens->type == pipe_type)
		return (printf("%s: `%s'\n", ERR_BASH, tokens->content), true);
	while (tokens)
	{
		prev_token = ft_find_node_n_position(tokens, tokens->position - 1);
		if (prev_token && prev_token->type == heredoc_delimiter)
		{
			tokens = tokens->next;
			continue ;
		}
		if (prev_token && tokens->type == pipe_type && (!tokens->next))
		{
			tokens = tokens->next;
			continue ;
		}
		if (prev_token && tokens->type == pipe_type && (!tokens->next
				|| tokens->next->type == pipe_type))
		{
			return (printf("%s: `%s'\n", ERR_BASH, tokens->content), true);
		}
		tokens = tokens->next;
	}
	return (false);
}

/**
 * @brief Verifies if there's an open pipe without a following command.
 *
 * @param tokens Linked list of token nodes.
 * @return true if an open pipe is detected at the end, false otherwise.
 */
bool	check_open_pipe_token(t_tokens *tokens)
{
	t_tokens	*current;
	t_tokens	*prew_token;

	current = tokens;
	while (current)
	{
		if (current->position > 0 && current->type == pipe_type)
		{
			prew_token = ft_find_node_n_position(tokens, current->position - 1);
			if (current->type == pipe_type && !current->next
				&& (prew_token->type == command || prew_token->type == argument
					|| prew_token->type == heredoc_delimiter))
			{
				return (printf(ERR_OPEN_PIPE), true);
			}
		}
		current = current->next;
	}
	return (false);
}

/**
 * @brief Validates tokens for consecutive redirections or pipes.
 *
 * @param tokens List of token nodes to be checked.
 * @return true if syntax errors are found, false otherwise.
 */
bool	check_bash_error_token(t_tokens *tokens)
{
	t_tokens	*next_token;
	t_tokens	*current;

	current = tokens;
	while (current)
	{
		next_token = current->next;
		if (is_redir(current) && next_token && is_redir(next_token))
		{
			return (printf("%s: `%s'\n", ERR_BASH, next_token->content), true);
		}
		if (current->type == pipe_type && next_token
			&& next_token->type == pipe_type)
		{
			return (printf("%s: `%s'\n", ERR_BASH, next_token->content), true);
		}
		current = next_token;
	}
	return (false);
}

/**
 * @brief Detects if redirection is not followed by a valid token.
 *
 * @param tokens List of token nodes.
 * @return true if redirection ends with a newline or a pipe,
 * false otherwise.
 */
// Modificamos el prototipo para que reciba t_all *all
bool check_bash_after_here(t_tokens *tokens, t_all *all)
{
    t_tokens *next_token;
    t_tokens *current;

	fprintf(stderr, "DEBUG (execute_command): Executing command:\n");
    fprintf(stderr, "  Path: \n");
    fprintf(stderr, "  Args:\n");
    for (int i = 0; all->exec->args && all->exec->args[i]; i++)
        fprintf(stderr, "    [%d]: \"%s\"\n", i, all->exec->args[i]);
    current = tokens;
    while (current)
    {
        next_token = current->next;
        // Caso 1: Redirección al final de la línea (sin token siguiente)
        if (is_redir(current) && (!next_token))
        {
            all->needs_more_input = true; // Establecer el flag de continuación
            return (false); // Indica que la línea actual no es válida por sí misma
        }
        // Caso 2: Redirección seguida inmediatamente por un pipe (error definitivo)
        if (is_redir(current) && (next_token) && next_token->type == pipe_type)
        {
            ft_error(all->meta->program_acces, ERR_BASH, next_token->content, 2);
            return (true); // Indica un error de sintaxis definitivo
        }
        current = next_token;
    }
    return (false); // No se encontró un error de sintaxis ni una necesidad de continuación
}

/**
 * @brief Aggregates all syntax checks related to pipes and heredocs.
 *
 * @param tokens List of tokens to check.
 * @param program Pointer to the main program structure for setting exit status.
 * @return true if no syntax errors are found, false otherwise.
 */
// Añadir t_all *all al prototipo
bool check_bash_pipe_error(t_tokens *tokens, t_program *program, t_all *all)
{
    all->needs_more_input = false; // Reiniciar el flag de continuación

    // Errores de sintaxis definitivos
    if (check_bash_pipe_before_here(tokens) || check_bash_before_here(tokens)
        || check_bash_error_token(tokens))
    {
        program->last_exit_status = 2;
        return (false);
    }

    // Error de pipe abierto, también definitivo
    if (check_open_pipe_token(tokens) == true)
    {
        program->last_exit_status = 1;
        return (false);
    }

    // Caso de redirección incompleta que puede necesitar más input
    if (check_bash_after_here(tokens, all) == true) // <-- Pasar 'all'
    {
        // Si check_bash_after_here devuelve true, es un error definitivo (ej. >|)
        program->last_exit_status = 2;
        return (false);
    }

    // Si all->needs_more_input es true, significa que es un error de continuación
    if (all->needs_more_input)
    {
        program->last_exit_status = 2; // Estado de salida para errores de sintaxis
        return (false); // Indica que la línea no es válida por sí misma, pero se puede continuar
    }

    return (true); // No se encontraron errores de sintaxis
}
