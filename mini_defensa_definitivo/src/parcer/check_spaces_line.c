/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_spaces_line.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzolotar <mzolotar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 10:36:16 by mzolotar          #+#    #+#             */
/*   Updated: 2025/06/07 09:18:52 by mzolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Copies an operator substring from the input line to a new line.
 *
 * Copies a detected operator from the input line into the new formatted line.
 * Updates the index in the new line accordingly.
 *
 * @param new_line The destination buffer for the formatted line.
 * @param meta Pointer to the metacharacter tracking structure.
 * @param line The original input line.
 * @param op_len Length of the operator to copy.
 */
void	copy_operator(char *new_line, t_metachars *meta, const char *line,
		int op_len)
{
	ft_strlcpy(&new_line[meta->j_index], &line[meta->i_index], op_len + 1);
	meta->j_index += op_len;
}

/**
 * @brief Processes and formats operators within a line.
 *
 * Checks if an operator is found at the current index. If so, adds
 * spaces before and after it if needed and copies the operator.
 *
 * @param line The original input line to process.
 * @param new_line The buffer where the formatted line is built.
 * @param meta Pointer to the metacharacter tracking structure.
 */
void	process_operators(const char *line, char *new_line, t_metachars *meta)
{
	int	k;
	int	op_len;

	k = 0;
	while (meta->allow_op[k])
	{
		op_len = ft_strlen(meta->allow_op[k]);
		if (is_operator(&line[meta->i_index], meta->allow_op[k]))
		{
			if (needs_space_before(line, meta->i_index))
				new_line[meta->j_index++] = ' ';
			copy_operator(new_line, meta, line, op_len);
			if (needs_space_after(line, meta->i_index, op_len))
				new_line[meta->j_index++] = ' ';
			meta->i_index += op_len - 1;
			return ;
		}
		k++;
	}
	new_line[meta->j_index++] = line[meta->i_index];
}

/**
 * @brief Processes an entire line, formatting operators with spacing.
 *
 * Iterates through the input line, adding spaces around operators outside
 * of quotes, and copying characters to the new formatted line.
 *
 * @param line The original input line.
 * @param new_line The resulting line with formatted operators.
 * @param meta Pointer to the metacharacter tracking structure.
 */
void	process_line(const char *line, char *new_line, t_metachars *meta)
{
	reset_meta_flags(meta, 3);
	while (line[meta->i_index])
	{
		toggle_quotes(line[meta->i_index], &meta->in_single_quote,
			&meta->in_double_quote);
		if (!meta->in_single_quote && !meta->in_double_quote)
			process_operators(line, new_line, meta);
		else
			new_line[meta->j_index++] = line[meta->i_index];
		meta->i_index++;
	}
	new_line[meta->j_index] = '\0';
}

/**
 * @brief Replaces the old line with a newly formatted version.
 *
 * Allocates memory and copies the formatted line into a new buffer.
 * Replaces the original line with the new one and frees the old memory.
 *
 * @param line Pointer to the original line to replace.
 * @param new_line The newly formatted line.
 * @param program Pointer to the program structure for error reporting.
 */
void	replace_line(char **line, char *new_line, t_program *program)
{
	char	*temp;

	temp = malloc(ft_strlen(new_line) + 1);
	if (!temp)
	{
		free(new_line);
		ft_error(program, MSG_ERR_MALLOC, NULL, 1);
		return ;
	}
	ft_strlcpy(temp, new_line, ft_strlen(new_line) + 1);
	free(*line);
	*line = temp;
	free(new_line);
}

/**
 * @brief Formats a command line by adding spacing around operators.
 *
 * Allocates memory for a new version of the line, processes operators,
 * and replaces the original line with the formatted version.
 *
 * @param line Pointer to the line to be formatted.
 * @param meta Pointer to the metacharacter tracking structure.
 * @param program Pointer to the program structure for error reporting.
 */
void	check_spaces_line(char **line, t_metachars *meta, t_program *program)
{
	char	*new_line;

	if (!line || !*line || !meta)
		return ;
	reset_meta_flags(meta, 3);
	new_line = allocate_new_line(*line, meta, program);
	if (!new_line)
		return ;
	process_line(*line, new_line, meta);
	replace_line(line, new_line, program);
}
