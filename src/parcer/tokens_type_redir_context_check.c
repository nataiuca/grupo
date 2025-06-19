/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_type_redir_context_check.c                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzolotar <mzolotar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 13:13:05 by mzolotar          #+#    #+#             */
/*   Updated: 2025/05/26 14:08:45 by mzolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Checks if a token is a command after a redirection and a valid path.
 *
 * @param current Pointer to the current token.
 * @param prev Pointer to the previous token.
 * @param head Pointer to the head of the token list.
 * @return true if it is a command after redirection with a path, false otherwise.
 */
bool	is_cmd_aft_redir_wit_prev_cmd(t_tokens *current, t_tokens *prev,
		t_tokens *head)
{
	if ((prev->type == path_name || prev->type == heredoc_delimiter)
		&& !is_redir(current) && !check_pipe(current)
		&& !any_command_before(prev, head))
		return (true);
	return (false);
}

/**
 * @brief Checks if the current token is a command after a redirection token.
 *
 * @param current Pointer to the current token.
 * @param prev Pointer to the previous token.
 * @return true if current is a command after redirection, false otherwise.
 */
bool	is_command_after_redir(t_tokens *current, t_tokens *prev)
{
	if (is_redir(prev) && !check_pipe(current) && !is_redir(current))
		return (true);
	return (false);
}

/**
 * @brief Checks if the current token is a command after a pipe token.
 *
 * @param current Pointer to the current token.
 * @param prev Pointer to the previous token.
 * @return true if current is a command after a pipe, false otherwise.
 */
bool	is_command_after_pipe(t_tokens *current, t_tokens *prev)
{
	if (check_pipe(prev) && !is_redir(current) && !check_pipe(current))
		return (true);
	return (false);
}

/**
 * @brief Checks if there is any command token before the given node.
 *
 * @param node Pointer to the token to start checking from.
 * @param head Pointer to the head of the token list.
 * @return true if a command exists before the node, false otherwise.
 */
bool	any_command_before(t_tokens *node, t_tokens *head)
{
	t_tokens	*current;

	current = head;
	while (current && current != node)
	{
		if (current->type == command)
			return (true);
		current = current->next;
	}
	return (false);
}

/**
 * @brief Checks if the token is the first command in the input sequence.
 *
 * @param current Pointer to the current token.
 * @return true if it is the first command, false otherwise.
 */
bool	is_first_command(t_tokens *current)
{
	if (current->position == 0 && !is_redir(current) && !check_pipe(current))
		return (true);
	return (false);
}
