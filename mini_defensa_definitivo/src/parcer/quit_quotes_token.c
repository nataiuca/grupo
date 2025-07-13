/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quit_quotes_token.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzolotar <mzolotar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 10:47:04 by mzolotar          #+#    #+#             */
/*   Updated: 2025/07/04 10:13:17 by mzolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Checks if the character is an outer quote and updates quote states.
 *
 * Determines whether the current character is an opening or closing quote
 * and updates the single or double quote flags accordingly.
 *
 * @param content The string being scanned for quotes.
 * @param i The current index within the string.
 * @param in_single_quote Pointer to the single quote state flag.
 * @param in_double_quote Pointer to the double quote state flag.
 * @return true if the character is an outer quote, false otherwise.
 */
bool	is_outer_quote(char *content, int i, bool *in_single_quote,
		bool *in_double_quote)
{
	if (content[i] == '\'' || content[i] == '"')
	{
		if (!(*in_single_quote) && !(*in_double_quote))
		{
			if (content[i] == '\'')
				*in_single_quote = true;
			else if (content[i] == '"')
				*in_double_quote = true;
			return (true);
		}
		else if ((*in_single_quote && content[i] == '\'') || (*in_double_quote
				&& content[i] == '"'))
		{
			*in_single_quote = false;
			*in_double_quote = false;
			return (true);
		}
	}
	return (false);
}

/**
 * @brief Determines if the quote at a given position is surrounded by text.
 *
 * Identifies quotes that are not used for grouping but are within words,
 * treating them as literal characters instead of delimiters.
 *
 * @param content The string to analyze.
 * @param i The current index of the quote character.
 * @return true if the quote is an inner quote, false otherwise.
 */
bool	is_inner_quote(char *content, int i)
{
	int	prev;
	int	next;

	if (i > 0 && content[i + 1] != '\0')
	{
		if ((content[i] == '\'' || content[i] == '"') && ft_isalnum(content[i
					- 1]) && ft_isalnum(content[i + 1]))
		{
			prev = i - 1;
			next = i + 1;
			while (prev >= 0 && content[prev] == ' ')
				prev--;
			while (content[next] != '\0' && content[next] == ' ')
				next++;
			if (prev >= 0 && next < i + 1)
				return (true);
		}
	}
	return (false);
}

/**
 * @brief Removes outer quotes from a token's content string.
 *
 * Iterates over the content of a token and eliminates surrounding quotes
 * while preserving inner quote characters that are part of words.
 *
 * @param current Pointer to the token whose quotes are to be removed.
 * @param meta Pointer to a t_metachars struct for parsing state.
 */
void	quit_quotes_token(t_tokens *current, t_metachars *meta)
{
	if (!current || !current->content || current->content[0] == '\0')
		return ;
	reset_meta_flags(meta, 3);
	while (current->content[meta->i_index] != '\0')
	{
		if (is_outer_quote(current->content, meta->i_index,
				&meta->in_single_quote, &meta->in_double_quote))
		{
			meta->i_index++;
			continue ;
		}
		if (is_inner_quote(current->content, meta->i_index))
		{
			meta->i_index++;
			continue ;
		}
		current->content[meta->j_index] = current->content[meta->i_index];
		meta->i_index++;
		meta->j_index++;
	}
	current->content[meta->j_index] = '\0';
	current->was_quoted = true;
}

/**
 * @brief Removes quotes from all tokens in a list, if properly closed.
 *
 * Traverses the linked list of tokens and strips quotes from token contents
 * if quotes are present and balanced (no unclosed quote).
 *
 * @param tokens Pointer to the head of the token list.
 * @param meta Pointer to a t_metachars struct for tracking quote states.
 */
void	check_and_quit_quotes_token(t_tokens *tokens, t_metachars *meta)
{
	t_tokens	*current;
	int			i;
	bool		is_quoted;

	current = tokens;
	while (current)
	{
		i = 0;
		reset_meta_flags(meta, 2);
		is_quoted = false;
		while (current->content[i] != '\0')
		{
			toggle_quotes(current->content[i], &meta->in_single_quote,
				&meta->in_double_quote);
			if (meta->in_single_quote || meta->in_double_quote)
				is_quoted = true;
			i++;
		}
		if (is_quoted && !meta->in_single_quote && !meta->in_double_quote)
		{
			quit_quotes_token(current, meta);
		}
		current = current->next;
	}
}
