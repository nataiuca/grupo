/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzolotar <mzolotar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 09:38:35 by mzolotar          #+#    #+#             */
/*   Updated: 2025/06/27 11:55:00 by mzolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**clone_env_array(char **envp, t_program *program)
{
	int		i;
	int		size;
	char	**copy;

	size = env_array_size(envp);
	copy = malloc(sizeof(char *) * (size + 1));
	if (!copy)
		return (ft_error(program, MSG_ERR_MALLOC, NULL, 1), NULL);
	i = 0;
	while (i < size)
	{
		copy[i] = ft_strdup(envp[i]);
		if (!copy[i])
		{
			while (--i >= 0)
				free(copy[i]);
			free(copy);
			return (NULL);
		}
		i++;
	}
	copy[i] = NULL;
	return (copy);
}

t_env	*init_env_node(t_program *program)
{
	t_env	*node;

	node = malloc(sizeof(t_env));
	if (!node)
	{
		ft_error(program, MSG_ERR_MALLOC, NULL, 1);
		return (NULL);
	}
	memset(node, 0, sizeof(t_env));
	node->key = NULL;
	node->value = NULL;
	node->next = NULL;
	return (node);
}

t_env	*parse_env_var_to_node(char *env_var, t_program *program)
{
	t_env	*node;
	char	*equal_sign;
	size_t	key_len;

	node = init_env_node(program);
	if (!node)
		return (NULL);
	equal_sign = ft_strchr(env_var, '=');
	node->is_export_only = 0;
	if (equal_sign)
	{
		key_len = equal_sign - env_var;
		node->key = ft_substr(env_var, 0, key_len);
		node->value = ft_strdup(equal_sign + 1);
	}
	else
	{
		node->key = ft_strdup(env_var);
		node->value = NULL;
		node->is_export_only = 1;
	}
	if (!node->key || (equal_sign && node->value == NULL))
		return (destroy_env_node(node), NULL);
	return (node);
}

t_env	*envp_array_to_list(char **envp, t_program *program)
{
	t_env	*head;
	t_env	*curr;
	t_env	*new_node;
	int		i;

	head = NULL;
	curr = NULL;
	i = 0;
	while (envp[i])
	{
		new_node = parse_env_var_to_node(envp[i], program);
		if (!new_node)
			return (destroy_env_list(head), NULL);
		if (!head)
			head = new_node;
		else
			curr->next = new_node;
		curr = new_node;
		i++;
	}
	return (head);
}
