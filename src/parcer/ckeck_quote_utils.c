/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ckeck_quote_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzolotar <mzolotar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 10:37:18 by mzolotar          #+#    #+#             */
/*   Updated: 2025/05/26 15:29:49 by mzolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Checks if the given string contains single or double quotes.
 *
 * @param str The string to be checked.
 * @return true if quotes are present, false otherwise.
 */
bool	is_quotes_present(const char *str)
{
	if (!str)
		return (false);
	if (ft_strchr(str, '"') || ft_strchr(str, '\''))
		return (true);
	return (false);
}

/**
 * @brief Determines if there are any unclosed quotes in the string.
 *
 * @param str The input string to evaluate.
 * @return true if quotes are left open, false otherwise.
 */
bool	quotes_is_open(const char *str)
{
	bool	single_quote;
	bool	double_quote;

	single_quote = false;
	double_quote = false;
	while (*str)
	{
		if (*str == '"' && !single_quote)
		{
			double_quote = !double_quote;
		}
		else if (*str == '\'' && !double_quote)
		{
			single_quote = !single_quote;
		}
		str++;
	}
	return (single_quote || double_quote);
}

/**
 * @brief Toggles quote flags based on the current character.
 *
 * @param c The character to evaluate for quote toggling.
 * @param in_single_quote Pointer to the single quote status flag.
 * @param in_double_quote Pointer to the double quote status flag.
 */
void	toggle_quotes(char c, bool *in_single_quote, bool *in_double_quote)
{
	if (c == '\'')
		*in_single_quote = !(*in_single_quote) && !(*in_double_quote);
	else if (c == '"')
		*in_double_quote = !(*in_double_quote) && !(*in_single_quote);
}

/**
 * @brief Updates the quote status in the meta structure based on the character.
 *
 * @param c The character to evaluate.
 * @param meta Pointer to the t_metachars struct containing quote flags.
 */
void	update_quote_status(const char c, t_metachars *meta)
{
	toggle_quotes(c, &meta->in_single_quote, &meta->in_double_quote);
}

/**
 * @brief Compares a string against a specific operator string.
 *
 * @param str The string to compare.
 * @param op The operator string to check against.
 * @return true if both strings match, false otherwise.
 */
bool	is_operator(const char *str, const char *op)
{
	return (ft_strncmp(str, op, ft_strlen(op)) == 0);
}
