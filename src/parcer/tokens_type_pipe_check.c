/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_type_pipe_check.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzolotar <mzolotar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 13:13:09 by mzolotar          #+#    #+#             */
/*   Updated: 2025/05/26 14:10:27 by mzolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Attempts to set the current token as a pipe and the next as command.
 *
 * If the current token is a pipe, assigns its type and checks if the next
 * token should be considered a command.
 *
 * @param current Pointer to the current token.
 * @param next_token Pointer to the next token in the list.
 * @return true if current is a pipe and type assignment succeeds, false otherwise.
 */
bool	try_set_as_pipe(t_tokens *current, t_tokens *next_token)
{
	if (check_pipe(current))
	{
		current->type = pipe_type;
		if (current->next && !check_pipe(next_token) && !is_redir(next_token))
		{
			current = current->next;
			current->type = command;
		}
		return (true);
	}
	return (false);
}

/**
 * @brief Checks whether the current token is a pipe symbol ("|").
 *
 * @param current Pointer to the current token.
 * @return true if the token is a pipe, false otherwise.
 */
bool	check_pipe(t_tokens *current)
{
	if (ft_strncmp(current->content, "|", -1) == 0)
		return (true);
	return (false);
}
