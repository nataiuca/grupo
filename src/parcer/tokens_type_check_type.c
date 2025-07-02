/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_type_check_type.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzolotar <mzolotar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 08:13:35 by mzolotar          #+#    #+#             */
/*   Updated: 2025/06/30 08:29:57 by mzolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	try_set_as_redirection(t_tokens *current, t_tokens *head, t_metachars *meta)
{
	(void)head;
	if (check_redir(current) != temp && is_redir(current))
	{
		current->type = check_redir(current);
		meta->redir_pending = true;
		//meta->expecting_cmd = !meta->cmd_seen;
		if (meta->cmd_seen)
			meta->expecting_cmd = false;
		else
			meta->expecting_cmd = true;
		return (true);
	}
	return (false);
}

bool	try_set_as_redir_follower(t_tokens *current, t_tokens *head, t_metachars *meta)
{
	t_tokens *prev;

	if (!current || !head)
		return (false);
	prev = ft_find_node_n_position(head, current->position - 1);
	if (!prev || !is_redir(prev))
		return (false);
	if (check_pipe(current) || is_redir(current))
		return (false);
	if (prev->type == redir_l_d)
		current->type = heredoc_delimiter;
	else if (prev->type == redir_l_u || prev->type == redir_r_u || prev->type == redir_r_d)
		current->type = path_name;
	else
		return (false);
	meta->redir_pending = false;
	meta->expecting_cmd = !meta->cmd_seen;
	return (true);
}

bool	assign_token_type(t_tokens *current, t_tokens *head, t_metachars *meta)
{
	t_tokens	*next_token;

	next_token = ft_find_node_n_position(head, current->position + 1);
	if (try_set_as_pipe(current, next_token, meta))
		return (true);
	if (try_set_as_redirection(current, head, meta))
		return (true);
	if (try_set_as_redir_follower(current, head, meta))
		return (true);
	if (try_set_as_command(current, head, meta))
		return (true);
	if (try_set_as_argument(current, head, meta))
		return (true);
	return (false);
}

void	process_tokens(t_tokens *tokens, bool *has_temp_tokens, t_metachars *meta)
{
	t_tokens	*current;

	current = tokens;
	while (current)
	{
		if (current->type == temp)
		{
			if (assign_token_type(current, tokens, meta))
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

//❌DEBUG, QUITAR
void debug_test(t_tokens *tokens)
{
	t_tokens *current = tokens;
	bool error_found = false;

	while (current)
	{
		if (current->type == temp)
		{
			fprintf(stderr, "\033[1;31m[ERROR] Token con tipo 'temp' sin asignar: posición %d, contenido: '%s'\033[0m\n", current->position, current->content);
			error_found = true;
		}
		current = current->next;
	}
	if (!error_found)
		return;
}



void	check_type_tokens(t_tokens *tokens, t_metachars *meta)
{
	bool	has_temp_tokens;

	t_tokens *original = tokens; //❌DEBUG, QUITAR
	
	if (!tokens)
		return ;
	has_temp_tokens = true;

	meta->cmd_seen = false;
	meta->pipe_seen = false;
	meta->expecting_cmd = false;
	meta->redir_pending = true;
	
	while (has_temp_tokens)
	{
		has_temp_tokens = false;
		process_tokens(tokens, &has_temp_tokens, meta);
	}
	



	
	debug_test(original); //❌DEBUG, QUITAR
}
