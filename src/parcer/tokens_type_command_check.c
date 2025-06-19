/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_type_command_check.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzolotar <mzolotar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 13:12:46 by mzolotar          #+#    #+#             */
/*   Updated: 2025/06/07 09:37:40 by mzolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Determines if the current token should be classified as a command.
 *
 * @param current Pointer to the current token node.
 * @param head Pointer to the head of the token list.
 * @return true if the token is a command, false otherwise.
 */
bool	check_command(t_tokens *current, t_tokens *head)
{
	t_tokens	*prev;

	prev = NULL;
	if (!current || current->type != temp)
		return (false);
	if (is_first_command(current))
		return (true);
	prev = ft_find_node_n_position(head, current->position - 1);
	if (!prev)
		return (false);
	if (is_command_after_pipe(current, prev))
		return (true);
	if (invalid_after_redir(current, head, redir_l_u))
		return (false);
	if (invalid_after_redir(current, head, redir_r_u)
		|| invalid_after_redir(current, head, redir_r_d))
		return (false);
	if (is_cmd_aft_redir_wit_prev_cmd(current, prev, head))
		return (true);
	if (is_command_after_redir(current, prev))
		return (true);
	return (false);
}

/**
 * @brief Checks if the current token can be considered an argument.
 *
 * @param current Pointer to the current token node.
 * @param head Pointer to the head of the token list.
 * @return true if the token is a valid argument, false otherwise.
 */
bool	check_argument(t_tokens *current, t_tokens *head)
{
	if (check_command(current, head) || check_pipe(current) || is_redir(current)
		|| current->type == path_name)
		return (false);
	if (is_after_redir_with_path(current, head, redir_l_u))
		return (false);
	if (is_after_redir_with_path(current, head, redir_r_u)
		|| is_after_redir_with_path(current, head, redir_r_d))
		return (false);
	return (true);
}


/**
 * @brief Attempts to set the token as a command if it meets command criteria.
 *
 * @param current Pointer to the current token node.
 * @param head Pointer to the head of the token list.
 * @return true if successfully set as command, false otherwise.
 */
bool	try_set_as_command(t_tokens *current, t_tokens *head)
{
	if (check_command(current, head))
	{
		current->type = command;
		return (true);
	}
	return (false);
}

/**
 * @brief Attempts to set the token as an argument if it qualifies.
 *
 * @param current Pointer to the current token node.
 * @param head Pointer to the head of the token list.
 * @return true if successfully set as argument, false otherwise.
 */
bool	try_set_as_argument(t_tokens *current, t_tokens *head)
{
	if (check_argument(current, head))
	{
		current->type = argument;
		return (true);
	}
	return (false);
}
