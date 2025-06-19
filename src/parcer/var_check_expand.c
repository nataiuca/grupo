/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_check_expand.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzolotar <mzolotar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 07:24:19 by mzolotar          #+#    #+#             */
/*   Updated: 2025/06/11 12:22:09 by mzolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Initializes a new dynamically allocated string for processing input.
 *
 * @param program Pointer to the main program structure (for error handling).
 * @param size Pointer to an integer to store the allocated buffer size.
 * @return A newly allocated empty string or NULL on allocation failure.
 */
char	*init_new_str(t_program *program, int *size)
{
	char	*new_str;

	*size = PROMPT_SIZE;
	new_str = ft_calloc(1, *size);
	if (!new_str)
	{
		return (ft_error(program, MSG_ERR_MALLOC, NULL, 1), NULL);
	}
	return (new_str);
}

/**
 * @brief Checks if the buffer has room for a null terminator.
 *
 * @param program Pointer to the program structure for error state tracking.
 * @param meta Pointer to the metadata tracking indices.
 * @param size The current size of the string buffer.
 * @return true if there is space, false otherwise.
 */
bool	has_space_for_null_terminator(t_program *program, t_metachars *meta,
		int size)
{
	if (meta->j_index >= size - 1)
	{
		printf("%s %d bytes.\n", ERR_VAR, PROMPT_SIZE * 3);
		program->last_exit_status = 1;
		return (false);
	}
	return (true);
}

/**
 * @brief Builds a new string from the original by expanding variables.
 *
 * @param original The original input string.
 * @param meta Pointer to the metadata used for parsing.
 * @param program Pointer to the main program structure.
 * @return A new string with variables expanded or NULL on failure.
 */
char	*build_new_content(const char *original, t_metachars *meta, t_program *program)
{
	int		size;
	char	*new_str;

	new_str = init_new_str(program, &size);
	if (!new_str)
		return (NULL);
	reset_meta_flags(meta, 3);
	while (original[meta->i_index] != '\0')
	{
		update_quote_status(original[meta->i_index], meta);
		new_str = process_character(original, meta, new_str, &size);
		if (!new_str)
			return (NULL);
	}
	if (!has_space_for_null_terminator(program, meta, size))
		return (free(new_str), NULL);
	new_str[meta->j_index] = '\0';
	return (new_str);
}

/**
 * @brief Expands all variables in a token's string if needed.
 *
 * @param str Pointer to the string to process and replace.
 * @param meta Pointer to parsing metadata.
 * @param program Pointer to the program structure for error handling.
 * @return true if expansion is successful or unnecessary, false otherwise.
 */
bool	check_and_expand_var(t_tokens *tokens, char **str, t_metachars *meta, t_program *program)
{
	char	*new_content;

	if (!str || !(*str))
		return (false);
	new_content = build_new_content(*str, meta, program);
	if (!new_content)
		return (false);
	if (ft_strncmp(*str, new_content, ft_strlen(*str) + 1) != 0)
	{
		free(*str);
		*str = new_content;
	}
	else
	{
		free(new_content);
	}
	tokens->from_expansion =true;
	return (true);
}

/**
 * @brief Expands variables in all tokens marked as expandable.
 *
 * @param tokens Pointer to the token list to check and expand.
 * @param meta Pointer to parsing metadata.
 * @param program Pointer to the program structure for errors.
 * @return true if all tokens are expanded correctly, false otherwise.
 */
bool	check_token_has_var(t_tokens *tokens, t_metachars *meta,
		t_program *program)
{
	while (tokens)
	{
		if (tokens->type == command || tokens->type == argument
			|| tokens->type == path_name)
		{
			if (check_and_expand_var(tokens, &tokens->content, meta, program) == false)
				return (false);

				
		}
		tokens = tokens->next;
	}
	return (true);
}
