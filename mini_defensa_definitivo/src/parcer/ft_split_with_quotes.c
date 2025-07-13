/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_with_quotes.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzolotar <mzolotar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 09:44:13 by mzolotar          #+#    #+#             */
/*   Updated: 2025/07/04 10:17:11 by mzolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Counts the number of tokens in a string, respecting quotes.
 *
 * Iterates through the input string and counts how many space-separated
 * tokens exist, while ignoring spaces inside single or double quotes.
 *
 * @param sts The input string to parse.
 * @param meta Pointer to a t_metachars struct for tracking quote states.
 * @return The number of tokens found in the input string.
 */
int	count_tokens(const char *sts, t_metachars *meta)
{
	int	count;
	int	i;

	count = 0;
	i = 0;
	reset_meta_flags(meta, 2);
	while (sts[i])
	{
		if (sts[i] == ' ' && !meta->in_single_quote && !meta->in_double_quote)
			i++;
		else
		{
			count++;
			while (sts[i] && (meta->in_single_quote || meta->in_double_quote
					|| sts[i] != ' '))
			{
				update_quote_status(sts[i], meta);
				i++;
			}
		}
	}
	return (count);
}

char	*extract_token(const char *sts, t_metachars *meta, t_program *program)
{
	int		length;
	char	*token;

	meta->x_index = meta->i_index;
	length = 0;
	reset_meta_flags(meta, 2);
	while (sts[meta->i_index])
	{
		if (sts[meta->i_index] == '\'' && !meta->in_double_quote)
			meta->in_single_quote = !meta->in_single_quote;
		else if (sts[meta->i_index] == '\"' && !meta->in_single_quote)
			meta->in_double_quote = !meta->in_double_quote;
		else if (sts[meta->i_index] == ' ' && !meta->in_single_quote
			&& !meta->in_double_quote)
			break ;
		meta->i_index++;
		length++;
	}
	token = malloc(length + 1);
	if (!token)
		return (ft_error(program, MSG_ERR_MALLOC, NULL, 1), NULL);
	ft_strncpy(token, &sts[meta->x_index], length);
	token[length] = '\0';
	return (token);
}

char	**ft_split_with_quotes(char const *sts, t_metachars *meta,
		t_program *program)
{
	char	**result;
	int		count;

	if (!sts || !meta)
		return (NULL);
	meta->i_index = 0;
	count = count_tokens(sts, meta);
	result = malloc((count + 1) * sizeof(char *));
	if (!result)
	{
		return (ft_error(program, MSG_ERR_MALLOC, NULL, 1), NULL);
	}
	meta->j_index = 0;
	while (sts[meta->i_index])
	{
		if (sts[meta->i_index] == ' ' && !meta->in_single_quote
			&& !meta->in_double_quote)
			meta->i_index++;
		else
			result[meta->j_index++] = extract_token(sts, meta, program);
	}
	result[meta->j_index] = NULL;
	return (result);
}
