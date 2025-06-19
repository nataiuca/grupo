/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_type_redir_check.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzolotar <mzolotar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 13:13:00 by mzolotar          #+#    #+#             */
/*   Updated: 2025/06/07 09:39:40 by mzolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Checks whether the current token represents a redirection symbol.
 *
 * @param current Pointer to the current token node.
 * @return true if the token is a redirection, false otherwise.
 */
bool	is_redir(t_tokens *current)
{
	if (ft_strncmp(current->content, "<", -1) == 0)
		return (true);
	else if (ft_strncmp(current->content, ">", -1) == 0)
		return (true);
	else if (ft_strncmp(current->content, ">>", -1) == 0)
		return (true);
	else if (ft_strncmp(current->content, "<<", -1) == 0)
		return (true);
	return (false);
}

/**
 * @brief Determines the type of redirection based on the token content.
 *
 * @param current Pointer to the current token node.
 * @return The redirection type or temp if not a redirection.
 */
t_type	check_redir(t_tokens *current)
{
	if (ft_strncmp(current->content, ">>", -1) == 0)
		return (redir_r_d);
	else if (ft_strncmp(current->content, "<<", -1) == 0)
		return (redir_l_d);
	else if (ft_strncmp(current->content, "<", -1) == 0)
		return (redir_l_u);
	else if (ft_strncmp(current->content, ">", -1) == 0)
		return (redir_r_u);
	return (temp);
}

/**
 * @brief Checks if a redirection is invalid due to its surrounding context.
 *
 * @param current Pointer to the current token node.
 * @param head Pointer to the head of the token list.
 * @param expected_redir Expected redirection type for validation.
 * @return true if the redirection is invalid, false otherwise.
 */
bool	invalid_after_redir(t_tokens *current, t_tokens *head,
		t_type expected_redir)
{
	t_tokens	*prev;
	t_tokens	*prev2;

	prev = ft_find_node_n_position(head, current->position - 1);
	prev2 = ft_find_node_n_position(head, current->position - 2);
	if (!prev || prev->type != path_name || current->position <= 1)
		return (false);
	if (prev2 && check_redir(prev2) == expected_redir)
		return (true);
	return (false);
}

/**
 * @brief Checks if a command appears after a redirection and path.
 *
 * @param current Pointer to the current token node.
 * @param head Pointer to the head of the token list.
 * @param expected_redir Expected redirection type before the command.
 * @return true if conditions are met, false otherwise.
 */
bool	is_after_redir_with_path(t_tokens *current, t_tokens *head,
		t_type expected_redir)
{
	t_tokens	*prev;
	t_tokens	*prev2;

	if (current->position < 2)
		return (false);
	prev = ft_find_node_n_position(head, current->position - 1);
	prev2 = ft_find_node_n_position(head, current->position - 2);
	if (prev && prev2 && prev->type == path_name
		&& check_redir(prev2) == expected_redir && !any_command_before(prev2,
			head))
	{
		current->type = command;
		return (true);
	}
	return (false);
}

/**
 * @brief Assigns the redirection type to the current token and its target.
 *
 * @param current Pointer to the current token node.
 * @param head Pointer to the head of the token list.
 */
void	assign_redirection_type(t_tokens *current, t_tokens *head)
{
	t_tokens	*next_token;

	next_token = ft_find_node_n_position(head, current->position + 1);
	current->type = check_redir(current);
	if (current->type == redir_l_d)
	{
		if (current->next != NULL && !check_pipe(next_token)
			&& !is_redir(next_token))
		{
			current = current->next;
			current->type = heredoc_delimiter;
		}
	}
	else if (current->type == redir_l_u || current->type == redir_r_u
		|| current->type == redir_r_d)
	{
		if (current->next != NULL && !check_pipe(next_token)
			&& !is_redir(next_token))
		{
			current = current->next;
			current->type = path_name;
		}
	}
}
