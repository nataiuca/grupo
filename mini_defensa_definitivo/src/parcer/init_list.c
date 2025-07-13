/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_list.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzolotar <mzolotar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 01:31:43 by mzolotar          #+#    #+#             */
/*   Updated: 2025/07/13 10:14:12 by mzolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Finds and returns the last node in a linked list of tokens.
 *
 * Iterates through the linked list until the last node is found.
 * If the list is empty, returns NULL.
 *
 * @param tokens Pointer to the first node in the token list.
 * @return Pointer to the last node in the list, or NULL if list is empty.
 */
t_tokens	*find_last_node(t_tokens *tokens)
{
	if (tokens == NULL)
		return (NULL);
	while (tokens->next)
	{
		tokens = tokens->next;
	}
	return (tokens);
}

/**
 * @brief Initializes a token node with default values.
 *
 * Sets default values for type, own_command, expand_here, and next.
 * Also assigns the node's position in the list.
 *
 * @param tokens Pointer to the token node to initialize.
 * @param position Integer representing the node's position in the list.
 */
static void	initialize_node(t_tokens *tokens, int position)
{
	if (tokens == NULL)
		return ;
	tokens->type = temp;
	tokens->expand_here = false;
	tokens->position = position;
	tokens->from_expansion = false;
	tokens->was_quoted = false;
	tokens->next = NULL;
}

/**
 * @brief Creates and adds a new node to the end of the token list.
 *
 * Allocates memory for a new token node, sets its content, and links it
 * to the end of the existing list. Reports memory errors if allocation fails.
 *
 * @param tokens Pointer to the head of the token list.
 * @param content The string content to assign to the new token node.
 * @param position The position index of the new node in the list.
 * @param program Pointer to the program structure for error handling.
 */
static int	add_node(t_tokens **tokens, char *content, int position,
		t_program *program)
{
	t_tokens	*node;
	t_tokens	*last_node;

	if (tokens == NULL)
		return (0);
	node = malloc(sizeof(t_tokens));
	if (!node)
		return (ft_error(program, MSG_ERR_MALLOC, NULL, 1), 0);
	node->content = ft_strdup(content);
	if (node->content == NULL)
	{
		free(node);
		return (ft_error(program, MSG_ERR_MALLOC, NULL, 1), 0);
	}
	initialize_node(node, position);
	if (*tokens == NULL)
		*tokens = node;
	else
	{
		last_node = find_last_node(*tokens);
		last_node->next = node;
	}
	return (1);
}

/**
 * @brief Initializes a token list from a string array.
 *
 * Iterates over an array of strings, creating and adding a token node
 * for each string to build the full linked list of tokens.
 *
 * @param tokens Pointer to the token list to initialize.
 * @param split_str Array of strings to convert into token nodes.
 * @param program Pointer to the program structure for error handling.
 */
void	init_list(t_tokens **tokens, char **split_str, t_program *program)
{
	int	i;

	i = 0;
	if (!split_str)
		return ;
	while (split_str[i])
	{
		if (!add_node(tokens, split_str[i], i, program))
		{
			free_tokens_list(tokens);
			*tokens = NULL;
			return ;
		}
		++i;
	}
}
