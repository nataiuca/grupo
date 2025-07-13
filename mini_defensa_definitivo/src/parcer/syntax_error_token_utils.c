/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_error_token_utils.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzolotar <mzolotar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 21:20:17 by mzolotar          #+#    #+#             */
/*   Updated: 2025/07/06 09:12:45 by mzolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Checks for the pattern: redirection followed by heredoc delimiter.
 *
 * @param tokens Pointer to the current token node.
 * @return true if the pattern matches a Bash syntax error, false otherwise.
 */
bool	is_problematic_redir_pattern(t_tokens *tokens)
{
	t_tokens	*next_next_token;

	if (!tokens->next || tokens->next->type != redir_l_d)
		return (false);
	next_next_token = ft_find_node_n_position(tokens, tokens->position + 2);
	if (!next_next_token)
		return (false);
	if (is_redir(tokens) && next_next_token->type == here_d)
	{
		printf("%s: `%s'\n", ERR_BASH, tokens->next->content);
		return (true);
	}
	return (false);
}

/**
 * @brief Detects if a pipe is followed by a heredoc at the beginning.
 *
 * @param tokens Pointer to the current token node.
 * @return true if pipe and heredoc are incorrectly placed at the start,
 * false otherwise.
 */
bool	is_pipe_heredoc_at_start(t_tokens *tokens)
{
	t_tokens	*next_next_token;

	if (tokens->type != pipe_type || tokens->position != 0)
		return (false);
	if (!tokens->next || tokens->next->type != redir_l_d)
		return (false);
	next_next_token = ft_find_node_n_position(tokens, tokens->position + 2);
	if (next_next_token && next_next_token->type == here_d)
	{
		printf("%s: `%s'\n", ERR_BASH, tokens->content);
		return (true);
	}
	return (false);
}

/**
 * @brief Main checker for invalid Bash heredoc patterns.
 *
 * @param tokens Linked list of token nodes to check.
 * @return true if an invalid Bash heredoc pattern is detected,
 * false otherwise.
 */
bool	check_bash_before_here(t_tokens *tokens)
{
	t_tokens	*last_token;

	last_token = find_last_node(tokens);
	while (tokens)
	{
		if ((is_redir(tokens) || tokens->type == pipe_type)
			&& last_token && last_token->position > 1)
		{
			if (is_problematic_redir_pattern(tokens))
				return (true);
			if (is_pipe_heredoc_at_start(tokens))
				return (true);
		}
		tokens = tokens->next;
	}
	return (false);
}
