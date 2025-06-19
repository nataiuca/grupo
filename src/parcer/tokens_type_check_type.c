/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_type_check_type.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzolotar <mzolotar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 08:13:35 by mzolotar          #+#    #+#             */
/*   Updated: 2025/05/26 14:02:53 by mzolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Attempts to classify the current token as a redirection if valid.
 *
 * @param current Pointer to the current token node.
 * @param head Pointer to the head of the token list.
 * @return true if the token was classified as a redirection, false otherwise.
 */
bool	try_set_as_redirection(t_tokens *current, t_tokens *head)
{
	if (check_redir(current) != temp && is_redir(current))
	{
		assign_redirection_type(current, head);
		return (true);
	}
	return (false);
}

/**
 * @brief Tries to classify a token as an argument if unused and in context.
 *
 * @param current Pointer to the current token node.
 * @param head Pointer to the head of the token list.
 * @return true if the token was marked as an argument, false otherwise.
 */
bool	try_set_as_argument_if_unused(t_tokens *current, t_tokens *head)
{
	if (any_command_before(current, head) && current->type == temp
		&& !check_pipe(current) && !is_redir(current))
	{
		current->type = argument;
		return (true);
	}
	return (false);
}

/**
 * @brief Determines and assigns a proper type to a temporary token.
 *
 * @param current Pointer to the current token to classify.
 * @param head Pointer to the head of the token list.
 * @return true if the token type was successfully assigned, false otherwise.
 */
bool	assign_token_type(t_tokens *current, t_tokens *head)
{
	t_tokens	*next_token;

	next_token = ft_find_node_n_position(head, current->position + 1);
	if (try_set_as_command(current, head))
		return (true);
	if (try_set_as_argument(current, head))
		return (true);
	if (try_set_as_redirection(current, head))
		return (true);
	if (try_set_as_pipe(current, next_token))
		return (true);
	if (try_set_as_argument_if_unused(current, head))
		return (true);
	return (false);
}

/**
 * @brief Iterates through token list and assigns types to temp tokens.
 *
 * @param tokens Pointer to the token list.
 * @param has_temp_tokens Pointer to flag indicating if temp tokens remain.
 */
void	process_tokens(t_tokens *tokens, bool *has_temp_tokens)
{
	t_tokens	*current;

	current = tokens;
	while (current)
	{
		if (current->type == temp)
		{
			if (assign_token_type(current, tokens))
				*has_temp_tokens = true;
		}
		current = current->next;
	}
	current = tokens;
	while (current)
	{
		if ((current->type == heredoc_delimiter)
			&& (!is_quotes_present(current->content)))
		{
			current->expand_here = true;
		}
		else
			current->expand_here = false;
		current = current->next;
	}
}

/**
 * @brief Main loop to process token list and classify all temporary tokens.
 *
 * @param tokens Pointer to the token list to be processed.
 */
void	check_type_tokens(t_tokens *tokens)
{
	bool	has_temp_tokens;

	if (!tokens)
		return ;
	has_temp_tokens = true;
	while (has_temp_tokens)
	{
		has_temp_tokens = false;
		process_tokens(tokens, &has_temp_tokens);
	}
}
