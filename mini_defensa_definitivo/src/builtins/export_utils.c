/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzolotar <mzolotar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 08:28:18 by mzolotar          #+#    #+#             */
/*   Updated: 2025/06/30 08:32:52 by mzolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*search_env_key(t_env *env, char *key)
{
	while (env)
	{
		if (ft_strcmp(env->key, key) == 0)
			return (env);
		env = env->next;
	}
	return (NULL);
}

void	set_env_value_by_key(t_env *env, char *key, char *new_value)
{
	t_env	*node;

	node = search_env_key(env, key);
	if (node)
	{
		if (node->value)
			free(node->value);
		if (new_value)
		{
			node->value = ft_strdup(new_value);
			node->is_export_only = 0;
		}
		else
		{
			node->value = NULL;
			node->is_export_only = 1;
		}
	}
}

void	parse_env_assignment(char *arg, char **key, char **value)
{
	int	i;

	i = 0;
	while (arg[i] && arg[i] != '=')
		i++;
	*key = ft_substr(arg, 0, i);
	if (!*key)
		return ;
	if (arg[i] == '=')
		*value = ft_strdup(arg + i + 1);
	else
		*value = NULL;
}

void	print_export_identifier_error(char *arg, t_program *program)
{
	ft_putstr_fd("bash: export: `", STDERR_FILENO);
	ft_putstr_fd(arg, STDERR_FILENO);
	ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
	program->last_exit_status = 1;
}

void	print_export_error(t_program *program, char **args)
{
	ft_putstr_fd("bash: export: ", STDERR_FILENO);
	ft_putstr_fd(args[1], STDERR_FILENO);
	ft_putstr_fd(": invalid option", STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
	ft_putstr_fd("Try 'export' with no options.\n", STDERR_FILENO);
	program->last_exit_status = 1;
}
