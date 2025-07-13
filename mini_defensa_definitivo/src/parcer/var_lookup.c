/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_lookup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzolotar <mzolotar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 15:09:15 by mzolotar          #+#    #+#             */
/*   Updated: 2025/07/04 10:16:30 by mzolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Extracts the variable name from a string starting with '$'.
 *
 * @param str The input string starting with a '$' character.
 * @param var_name The buffer where the extracted variable name is stored.
 * @return The number of characters parsed from the input string.
 */
int	extract_var_name(const char *str, char *var_name)
{
	int	i;
	int	j;

	i = 1;
	if (str[i] == '\0')
		return (0);
	if (str[i] == '?')
	{
		var_name[0] = '?';
		var_name[1] = '\0';
		return (2);
	}
	if (!(str[i] == '_' || ft_isalpha(str[i])))
		return (0);
	var_name[0] = str[i];
	i++;
	j = 1;
	while (str[i] != '\0' && (str[i] == '_' || ft_isalnum(str[i])))
	{
		var_name[j++] = str[i++];
	}
	var_name[j] = '\0';
	return (i);
}

/**
 * @brief Expands an environment variable or special shell variable.
 *
 * @param program A pointer to the `t_program` structure.
 * @param var_name The name of the variable to expand.
 * @return A pointer to the corresponding value or an empty string if not found.
 */
char	*expand_var(t_program *program, char *var_name)
{
	t_env		*found;
	static char	exit_status_buffer[12];

	if (ft_strncmp(var_name, "$", 2) == 0)
		return ("$");
	if (ft_strncmp(var_name, "?", 2) == 0)
	{
		ft_itoa_without_malloc(get_or_set_last_exit_status(0, false),
			exit_status_buffer, sizeof(exit_status_buffer));
		return (exit_status_buffer);
	}
	if (ft_strncmp(var_name, "HOSTNAME", 8) == 0)
		return (get_hostname());
	if (!(var_name[0] == '_' || ft_isalpha(var_name[0])))
		return (var_name);
	found = search_env_key(program->env, var_name);
	if (found && found->value)
		return (found->value);
	return ("");
}
