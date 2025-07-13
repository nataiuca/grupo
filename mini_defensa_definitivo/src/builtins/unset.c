/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzolotar <mzolotar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 12:44:25 by mzolotar          #+#    #+#             */
/*   Updated: 2025/06/30 08:33:49 by mzolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_key(char *str)
{
	int	i;

	i = 0;
	if (!str || !ft_isalpha(str[0]))
		return (0);
	while (str[i])
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

int	delete_env_node(t_env **env, const char *key)
{
	t_env	*current;
	t_env	*prev;

	current = *env;
	prev = NULL;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			if (prev == NULL)
				*env = current->next;
			else
				prev->next = current->next;
			free(current->key);
			free(current->value);
			free(current);
			return (0);
		}
		prev = current;
		current = current->next;
	}
	return (1);
}

static	void	print_unset_error(char *arg, t_program *program)
{
	ft_putstr_fd("bash: unset: `", STDERR_FILENO);
	ft_putstr_fd(arg, STDERR_FILENO);
	ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
	ft_putstr_fd("Try 'unset' with no options.\n", STDERR_FILENO);
	program->last_exit_status = 1;
}

int	ft_unset(t_all *all, char **args, t_program *program)
{
	int	i;
	int	err;

	i = 1;
	err = 0;
	(void)all;
	if (!args || !args[1])
		return (err);
	while (args[i])
	{
		if (!check_key(args[i]))
		{
			print_unset_error(args[i], program);
			err = 1;
		}
		else if (delete_env_node(&program->env, args[i]) == 0)
			update_envp_copy(program);
		i++;
	}
	program->last_exit_status = 0;
	return (err);
}
