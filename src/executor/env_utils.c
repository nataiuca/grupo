/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzolotar <mzolotar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 11:53:00 by mzolotar          #+#    #+#             */
/*   Updated: 2025/06/30 08:34:19 by mzolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**malloc_env_array(t_env *env, t_program *program)
{
	int		count;
	char	**matrix;

	count = env_list_length(env);
	matrix = malloc(sizeof(char *) * (count + 1));
	if (!matrix)
	{
		ft_error(program, MSG_ERR_MALLOC, NULL, 1);
		return (NULL);
	}
	return (matrix);
}

char	**build_envp_from_list(t_env *env, t_program *program)
{
	char	**matrix;
	char	*temp;
	int		i;

	matrix = malloc_env_array(env, program);
	if (!matrix)
		return (NULL);
	i = 0;
	while (env)
	{
		temp = ft_strjoin(env->key, "=");
		if (env->value)
			matrix[i] = ft_strjoin(temp, env->value);
		else
			matrix[i] = ft_strdup(temp);
		free(temp);
		if (!matrix[i])
			return (free_split_strs(matrix), NULL);
		i++;
		env = env->next;
	}
	matrix[i] = NULL;
	return (matrix);
}

int	env_list_length(t_env *env)
{
	int	count;

	count = 0;
	while (env)
	{
		count++;
		env = env->next;
	}
	return (count);
}

int	env_array_size(char **envp)
{
	int	size;

	size = 0;
	if (!envp)
		return (0);
	while (envp[size])
		size++;
	return (size);
}

void	update_envp_copy(t_program *program)
{
	if (!program)
		return ;
	if (program->envp_copy)
		free_split_strs(program->envp_copy);
	program->envp_copy = build_envp_from_list(program->env, program);
	if (!program->envp_copy)
		return ;
}
