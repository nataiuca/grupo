/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_structures_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzolotar <mzolotar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 11:59:48 by mzolotar          #+#    #+#             */
/*   Updated: 2025/07/13 11:15:47 by mzolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Initializes the split string arrays within the `t_metachars`
 * structure, used for recognizing operators and special characters.
 *
 * @param meta A pointer to the `t_metachars` structure to initialize.
 * @param program A pointer to the `t_program` for error reporting.
 * @return True on successful initialization, false otherwise (e.g., malloc
 * failure).
 */
bool	init_meta_splits(t_metachars *meta, t_program *program)
{
	meta->allow_op = ft_split(" << | >> < > ", ' ');
	if (!meta->allow_op)
	{
		free(meta);
		ft_error(program, MSG_ERR_MALLOC, NULL, 1);
		return (false);
	}
	meta->var = ft_split(" $ ", ' ');
	if (!meta->var)
	{
		free_split_strs(meta->allow_op);
		free(meta);
		ft_error(program, MSG_ERR_MALLOC, NULL, 1);
		return (false);
	}
	meta->unexpected_token = ft_split(" `newline' `|' `>' `<' `>>' `<<' ", ' ');
	if (!meta->unexpected_token)
	{
		free_split_strs(meta->allow_op);
		free_split_strs(meta->var);
		free(meta);
		ft_error(program, MSG_ERR_MALLOC, NULL, 1);
		return (false);
	}
	return (true);
}

void	free_split_strs(char **split_strs)
{
	int	i;

	if (!split_strs)
		return ;
	i = 0;
	while (split_strs[i])
	{
		free(split_strs[i]);
		split_strs[i] = NULL;
		i++;
	}
	free(split_strs);
	split_strs = NULL;
}

void	free_tokens_list(t_tokens **tokens)
{
	t_tokens	*current;
	t_tokens	*next_node;

	if (!tokens || !*tokens)
		return ;
	current = *tokens;
	while (current)
	{
		next_node = current->next;
		if (current->content)
			free(current->content);
		free(current);
		current = next_node;
	}
	*tokens = NULL;
}

void	free_child(t_program *program, t_all *all)
{
	if (all)
		free_all_structs(all, 1);
	if (program->env)
	{
		destroy_env_list(program->env);
		program->env = NULL;
	}
	if (program->envp_copy)
	{
		free_split_strs(program->envp_copy);
		program->envp_copy = NULL;
	}
	if (program->prompt_hostname)
		free(program->prompt_hostname);
}
