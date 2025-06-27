/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_type_command_check.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzolotar <mzolotar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 13:12:46 by mzolotar          #+#    #+#             */
/*   Updated: 2025/06/26 12:28:16 by mzolotar         ###   ########.fr       */
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
		return false;
	if (current->type == command)
		return true;
	if (is_first_command(current))
		return true;

	prev = ft_find_node_n_position(head, current->position - 1);
	if (!prev)
		return false;

	if (is_command_after_pipe(current, prev))
		return true;

	if (invalid_after_redir(current, head, redir_l_u) ||
		invalid_after_redir(current, head, redir_r_u) ||
		invalid_after_redir(current, head, redir_r_d))
		return false;

	return false;
}

/*
bool	try_set_as_command(t_tokens *current, t_tokens *head, t_metachars *meta)
{
	(void)head;
	if (check_command(current, head, meta))
	{
		current->type = command;
		meta->cmd_seen = true;
		meta->expecting_cmd = false;
		return (true);
	}
	//fprintf(stderr, "command: %s, expecting_cmd: %d, cmd_seen: %d \n", current->content, meta->expecting_cmd, meta->cmd_seen);
	if (meta->expecting_cmd && !meta->cmd_seen && current->type == temp)
	{
		current->type = command;
		meta->cmd_seen = true;
		meta->expecting_cmd = false;
		return (true);
	}
	if (meta->expecting_cmd && !meta->cmd_seen && meta->pipe_seen && !meta->redir_pending && current->type == temp)
	{
		current->type = command;
		meta->cmd_seen = true;
		meta->expecting_cmd = false;
		return (true);
	}
	return (false);
}
	*/
bool	try_set_as_command(t_tokens *current, t_tokens *head, t_metachars *meta)
{
	(void)head;
	if (!current || current->type != temp)
		return false;

	// Si check_command da positivo por heurística → se asigna como command
	if (check_command(current, head, meta))
	{
		current->type = command;
		meta->cmd_seen = true;
		meta->expecting_cmd = false;
		return true;
	}

	// Fallback: confiar en flags si se espera un comando
	if (meta->expecting_cmd && !is_redir(current) && !check_pipe(current))
	{
		current->type = command;
		meta->cmd_seen = true;
		meta->expecting_cmd = false;
		return true;
	}

	return false;
}


bool	try_set_as_argument(t_tokens *current, t_tokens *head, t_metachars *meta)
{
	(void)head;
	if (check_pipe(current) || is_redir(current) || current->type == path_name || current->type == heredoc_delimiter || current->type == command)
		return (false);
	//if (current->type == temp && meta->cmd_seen && !meta->expecting_cmd && !meta->redir_pending)
	//if ( meta->cmd_seen && !meta->expecting_cmd && !meta->redir_pending && current->type == temp)
	//fprintf(stderr, "current->content en arg: %s, cmd_seen: %d, expecting_cmd: %d \n ", current->content, meta->cmd_seen, meta->expecting_cmd);
	if ( meta->cmd_seen && !meta->expecting_cmd && current->type == temp)
	{
		current->type = argument;
		return (true);
	}
	return (false);
}

