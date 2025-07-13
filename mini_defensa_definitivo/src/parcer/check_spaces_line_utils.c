/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_spaces_line_utils.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzolotar <mzolotar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 13:51:54 by mzolotar          #+#    #+#             */
/*   Updated: 2025/07/04 10:14:08 by mzolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Checks if a space is needed before the current character.
 *
 * Determines whether a space should be added before an operator,
 * based on the previous character.
 *
 * @param line The input command line.
 * @param i Current index in the line.
 * @return true if a space is needed before, false otherwise.
 */
bool	needs_space_before(const char *line, int i)
{
	return (i > 0 && line[i - 1] != ' ');
}

/**
 * @brief Checks if a space is needed after the current operator.
 *
 * Determines whether a space should be added after an operator,
 * based on the next character in the line.
 *
 * @param line The input command line.
 * @param i Index where the operator starts.
 * @param op_len Length of the operator.
 * @return true if a space is needed after, false otherwise.
 */
bool	needs_space_after(const char *line, int i, int op_len)
{
	return (line[i + op_len] != '\0' && line[i + op_len] != ' ');
}

/**
 * @brief Counts extra spaces required around an operator.
 *
 * Calculates how many additional spaces are needed before and after
 * an operator at the current position.
 *
 * @param line The input command line.
 * @param meta Pointer to metacharacter handling structure.
 * @return Number of extra spaces needed for the current operator.
 */
int	count_extra_spaces(const char *line, t_metachars *meta)
{
	int	k;
	int	op_len;
	int	extra_spaces;

	extra_spaces = 0;
	k = 0;
	while (meta->allow_op[k] != NULL)
	{
		op_len = ft_strlen(meta->allow_op[k]);
		if ((is_operator(&line[meta->i_index], meta->allow_op[k])))
		{
			if (needs_space_before(line, meta->i_index))
				extra_spaces++;
			if (needs_space_after(line, meta->i_index, op_len))
				extra_spaces++;
		}
		k++;
	}
	return (extra_spaces);
}

/**
 * @brief Calculates total extra spaces needed for all operators.
 *
 * Iterates through the line, excluding quotes, and sums the extra
 * spaces required to separate each detected operator.
 *
 * @param line The input command line.
 * @param meta Pointer to metacharacter handling structure.
 * @return Total number of extra spaces to allocate.
 */
int	count_spaces_for_ops(const char *line, t_metachars *meta)
{
	int	extra_spaces;

	extra_spaces = 0;
	meta->i_index = 0;
	reset_meta_flags(meta, 2);
	while (line[meta->i_index])
	{
		update_quote_status(line[meta->i_index], meta);
		if (!meta->in_single_quote && !meta->in_double_quote)
		{
			extra_spaces += count_extra_spaces(line, meta);
		}
		meta->i_index++;
	}
	return (extra_spaces);
}

/**
 * @brief Allocates a new line with space for operators.
 *
 * Allocates a new string large enough to hold the original line
 * plus the required extra spaces for operator separation.
 *
 * @param line The input command line.
 * @param meta Pointer to metacharacter handling structure.
 * @param program Pointer to the program structure for error handling.
 * @return A pointer to the newly allocated string, or NULL on failure.
 */
char	*allocate_new_line(const char *line, t_metachars *meta,
		t_program *program)
{
	int		extra_spaces;
	char	*new_line;

	extra_spaces = count_spaces_for_ops(line, meta);
	if (extra_spaces == 0)
		return (NULL);
	new_line = malloc(ft_strlen(line) + extra_spaces + 1);
	if (!new_line)
	{
		ft_error(program, MSG_ERR_MALLOC, NULL, 1);
		return (NULL);
	}
	return (new_line);
}
