/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_expand_core.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzolotar <mzolotar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 15:09:01 by mzolotar          #+#    #+#             */
/*   Updated: 2025/07/10 09:03:53 by mzolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Processes a character and handles variable expansion if needed.
 *
 * @param original The original input string.
 * @param meta Pointer to metadata tracking parse state.
 * @param new_str Pointer to the buffer being built.
 * @param new_size Pointer to the current buffer size.
 * @return The updated string buffer or NULL on failure.
 */
char	*process_character(const char *original, t_metachars *meta,
		char *new_str, int *new_size)
{
	if (original[meta->i_index] == '$' && !meta->in_single_quote)
	{
		if (original[meta->i_index + 1] == '_'
			|| ft_isalpha(original[meta->i_index + 1])
			|| original[meta->i_index + 1] == '?')
		{
			handle_expansion(original, meta, &new_str, new_size);
			if (!new_str)
				return (NULL);
			return (new_str);
		}
		else
		{
			new_str[meta->j_index++] = '$';
			meta->i_index++;
		}
	}
	else
	{
		new_str[meta->j_index++] = original[meta->i_index++];
	}
	return (new_str);
}

/**
 * @brief Handles expansion logic when a variable is detected.
 *
 * @param original The original input string.
 * @param meta Pointer to parsing metadata.
 * @param new_str Pointer to the string being constructed.
 * @param new_size Pointer to the current buffer size.
 */
void	handle_expansion(const char *original, t_metachars *meta,
		char **new_str, int *new_size)
{
	int		len;
	char	*var_value;

	var_value = get_valid_var_value(meta->program_acces, original, meta, &len);
	if (!var_value)
		return ;
	if (!append_var_value_to_str(new_str, new_size, var_value, meta))
		return ;
	meta->i_index += len;
}

/**
 * @brief Appends a variable value to the resulting string.
 *
 * @param new_str Pointer to the string buffer.
 * @param new_size Pointer to the current size of the buffer.
 * @param var_value The variable value to append.
 * @param meta Pointer to parsing metadata.
 * @return true on success, false if memory allocation fails.
 */
bool	append_var_value_to_str(char **new_str, int *new_size, char *var_value,
		t_metachars *meta)
{
	int	var_len;

	var_len = ft_strlen(var_value);
	if (meta->j_index + var_len >= *new_size)
	{
		*new_str = update_string_size(*new_str, new_size, var_len);
		if (!(*new_str))
			return (false);
	}
	if (**new_str == '\0')
		ft_strlcat(*new_str, "", *new_size);
	ft_strlcat(*new_str, var_value, *new_size);
	meta->j_index += var_len;
	return (true);
}

/**
 * @brief Retrieves and expands a valid variable from the original string.
 *
 * @param program A pointer to the `t_program` structure.
 * @param original The original string.
 * @param meta Pointer to parsing metadata.
 * @param len Pointer to store the number of characters consumed.
 * @return The value of the variable or an empty string if not found.
 */
char	*get_valid_var_value(t_program *program, const char *original,
		t_metachars *meta, int *len)
{
	char	var_name[HOSTNAME_SIZE];
	char	*var_value;

	*len = extract_var_name(original + meta->i_index, var_name);
	if (*len <= 0)
		return (NULL);
	var_value = expand_var(program, var_name);
	if (!var_value)
		var_value = "";
	return (var_value);
}

/**
 * @brief Reallocates the string buffer to accommodate more characters.
 *
 * @param new_str The current string buffer to resize.
 * @param new_size Pointer to update with the new buffer size.
 * @param min_extra_size Minimum space needed to append.
 * @return A pointer to the new resized buffer or NULL on failure.
 */
char	*update_string_size(char *new_str, int *new_size, int min_extra_size)
{
	int		old_size;
	int		copied;
	char	*temp;

	old_size = *new_size;
	*new_size = ((*new_size + min_extra_size) * 2);
	if (*new_size > PROMPT_SIZE)
		*new_size = PROMPT_SIZE * 3;
	if (*new_size > 6141)
		return (write(STDERR_FILENO, "error: arg_max\n", 15), NULL);
	temp = malloc(*new_size);
	if (!temp)
	{
		free(new_str);
		return (NULL);
	}
	copied = ft_strlcpy(temp, new_str, old_size);
	if (copied >= old_size)
	{
		free(new_str);
		return (NULL);
	}
	free(new_str);
	return (temp);
}
