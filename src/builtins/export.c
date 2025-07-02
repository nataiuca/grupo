/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzolotar <mzolotar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 08:28:24 by mzolotar          #+#    #+#             */
/*   Updated: 2025/06/30 08:33:07 by mzolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "minishell.h"

void write_export(t_env *env, int fd)
{
	t_env *tmp;

	tmp = env;
	while (tmp)
	{
		ft_putstr_fd("declare -x ", fd);
		ft_putstr_fd(tmp->key, fd);
		if (tmp->value != NULL)
		{
			ft_putstr_fd("=\"", fd);
			ft_putstr_fd(tmp->value, fd);
			ft_putstr_fd("\"\n", fd);
		}
		else
			ft_putstr_fd("\n", fd);
		tmp = tmp->next;
	}
}

int contains_equal(char *arg)
{
	int i = 0;

	while (arg[i])
	{
		if (arg[i] == '=')
			return (1);
		i++;
	}
	return (0);
}

void update_or_add_env(t_program *program, char *key, char *value, int has_equal)
{
	t_env *node = search_env_key(program->env, key);

	if (node)
	{
		if (has_equal)
		{
			set_env_value_by_key(program->env, key, value);
		}
		else if (node->value == NULL)
		{
			node->is_export_only = 1;
		}
	}
	else
	{
		append_new_env_var(&program->env, key, value);
	}
}

void parse_and_export_var(t_program *program, char *arg)
{
	char *key;
	char *value;
	int has_equal;

	key = NULL;
	value = NULL;
	has_equal = contains_equal(arg);
	parse_env_assignment(arg, &key, &value);
	if (!check_key(key))
		print_export_identifier_error(arg, program);
	else
		update_or_add_env(program, key, value, has_equal);
	free(key);
	if (value)
		free(value);
}

int	ft_export(t_all *all, char **args, t_program *program)
{
	int	i;
	int	out;
	
	i = 1;
	if (!args || !args[0])
		return (1);
	if (all->exec->outfile != -1)
		out = all->exec->outfile;
	else
		out = STDOUT_FILENO;
	if (!args[1])
	{
		write_export(program->env, out);
		return (program->last_exit_status = 0, 0);
	}
	if (args[1][0] == '-')
	{
		return (print_export_error(program, args), 1);
	}
	while (args[i])
		parse_and_export_var(program, args[i++]);
	update_envp_copy(program);
	return (program->last_exit_status = 0, 0);
}
