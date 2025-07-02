/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_type_command_check.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzolotar <mzolotar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 13:12:46 by mzolotar          #+#    #+#             */
/*   Updated: 2025/06/30 08:30:11 by mzolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_command_after_pipe(t_tokens *current, t_tokens *prev)
{
	if (check_pipe(prev) && !is_redir(current) && !check_pipe(current))
		return (true);
	return (false);
}

bool	is_first_command(t_tokens *current)
{
	if (current->position == 0 && !is_redir(current) && !check_pipe(current))
		return (true);
	return (false);
}

bool	check_command(t_tokens *current, t_tokens *head, t_metachars *meta)
{
	t_tokens *prev;

	(void)meta;
	if (!current)
		return (false);
	if (current->type == command)
		return (true);
	if (is_first_command(current))
		return true;
	prev = ft_find_node_n_position(head, current->position - 1);
	if (!prev)
		return (false);
	if (is_command_after_pipe(current, prev))
		return (true);
	if (invalid_after_redir(current, head, redir_l_u) ||
		invalid_after_redir(current, head, redir_r_u) ||
		invalid_after_redir(current, head, redir_r_d))
		return (false);
	return (false);
}

bool	try_set_as_command(t_tokens *current, t_tokens *head, t_metachars *meta)
{
	(void)head;
	if (!current || current->type != temp)
		return (false);
	if (check_command(current, head, meta))
	{
		current->type = command;
		meta->cmd_seen = true;
		meta->expecting_cmd = false;
		return (true);
	}
	if (meta->expecting_cmd && !is_redir(current) && !check_pipe(current))
	{
		current->type = command;
		meta->cmd_seen = true;
		meta->expecting_cmd = false;
		return (true);
	}
	return (false);
}

bool	try_set_as_argument(t_tokens *current, t_tokens *head, t_metachars *meta)
{
	(void)head;
	if (check_pipe(current) || is_redir(current) || current->type == path_name || current->type == heredoc_delimiter || current->type == command)
		return (false);
	if ( meta->cmd_seen && !meta->expecting_cmd && current->type == temp)
	{
		current->type = argument;
		return (true);
	}
	return (false);
}
