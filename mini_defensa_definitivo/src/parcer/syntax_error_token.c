/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_error_token.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzolotar <mzolotar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 11:22:55 by mzolotar          #+#    #+#             */
/*   Updated: 2025/07/06 09:12:31 by mzolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Checks if a pipe appears in a syntactically invalid position.
 *
 * @param tokens Pointer to the token list.
 * @return true if pipe misuse is found, false otherwise.
 */
bool	check_bash_pipe_before_here(t_tokens *tokens)
{
	t_tokens	*prev_token;

	if (tokens && tokens->type == pipe_type)
		return (printf("%s: `%s'\n", ERR_BASH, tokens->content), true);
	while (tokens)
	{
		prev_token = ft_find_node_n_position(tokens, tokens->position - 1);
		if (prev_token && prev_token->type == here_d)
		{
			tokens = tokens->next;
			continue ;
		}
		if (prev_token && tokens->type == pipe_type && (!tokens->next))
		{
			tokens = tokens->next;
			continue ;
		}
		if (prev_token && tokens->type == pipe_type && (!tokens->next
				|| tokens->next->type == pipe_type))
		{
			return (printf("%s: `%s'\n", ERR_BASH, tokens->content), true);
		}
		tokens = tokens->next;
	}
	return (false);
}

/**
 * @brief Verifies if there's an open pipe without a following command.
 *
 * @param tokens Linked list of token nodes.
 * @return true if an open pipe is detected at the end, false otherwise.
 */
bool	check_open_pipe_token(t_tokens *tokens)
{
	t_tokens	*current;
	t_tokens	*prew_token;

	current = tokens;
	while (current)
	{
		if (current->position > 0 && current->type == pipe_type)
		{
			prew_token = ft_find_node_n_position(tokens, current->position - 1);
			if (current->type == pipe_type && !current->next
				&& (prew_token->type == command || prew_token->type == argument
					|| prew_token->type == here_d))
			{
				return (printf(ERR_OPEN_PIPE), true);
			}
		}
		current = current->next;
	}
	return (false);
}

/**
 * @brief Validates tokens for consecutive redirections or pipes.
 *
 * @param tokens List of token nodes to be checked.
 * @return true if syntax errors are found, false otherwise.
 */
bool	check_bash_error_token(t_tokens *tokens)
{
	t_tokens	*next_token;
	t_tokens	*current;

	current = tokens;
	while (current)
	{
		next_token = current->next;
		if (is_redir(current) && next_token && is_redir(next_token))
		{
			return (printf("%s: `%s'\n", ERR_BASH, next_token->content), true);
		}
		if (current->type == pipe_type && next_token
			&& next_token->type == pipe_type)
		{
			return (printf("%s: `%s'\n", ERR_BASH, next_token->content), true);
		}
		current = next_token;
	}
	return (false);
}

/**
 * @brief Detects if redirection is not followed by a valid token.
 *
 * @param tokens List of token nodes.
 * @return true if redirection ends with a newline or a pipe,
 * false otherwise.
 */
bool	check_bash_after_here(t_tokens *tokens)
{
	t_tokens	*next_token;
	t_tokens	*current;

	current = tokens;
	while (current)
	{
		next_token = current->next;
		if (is_redir(current) && (!next_token))
		{
			return (printf("%s: `newline'\n", ERR_BASH), true);
		}
		if (is_redir(current) && (next_token) && next_token->type == pipe_type)
		{
			return (printf("%s: `%s'\n", ERR_BASH, next_token->content), true);
		}
		current = next_token;
	}
	return (false);
}

/**
 * @brief Aggregates all syntax checks related to pipes and heredocs.
 *
 * @param tokens List of tokens to check.
 * @param program Pointer to the main program structure for setting exit status.
 * @return true if no syntax errors are found, false otherwise.
 */
bool	check_bash_pipe_error(t_tokens *tokens, t_program *program)
{
	if (check_bash_pipe_before_here(tokens) || check_bash_before_here(tokens)
		|| check_bash_error_token(tokens))
	{
		program->last_exit_status = 2;
		return (false);
	}
	if (check_open_pipe_token(tokens) == true)
	{
		program->last_exit_status = 1;
		return (false);
	}
	if (check_bash_after_here(tokens) == true)
	{
		program->last_exit_status = 2;
		return (false);
	}
	return (true);
}
