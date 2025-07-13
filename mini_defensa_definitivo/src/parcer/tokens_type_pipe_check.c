/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_type_pipe_check.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzolotar <mzolotar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 13:13:09 by mzolotar          #+#    #+#             */
/*   Updated: 2025/06/26 09:25:05 by mzolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	try_set_as_pipe(t_tokens *current, t_tokens *next_token,
			t_metachars *meta)
{
	(void)next_token;
	if (check_pipe(current))
	{
		current->type = pipe_type;
		meta->pipe_seen = true;
		meta->cmd_seen = false;
		meta->expecting_cmd = true;
		meta->redir_pending = true;
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
