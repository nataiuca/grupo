/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_structures_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzolotar <mzolotar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 11:59:48 by mzolotar          #+#    #+#             */
/*   Updated: 2025/06/07 09:24:09 by mzolotar         ###   ########.fr       */
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
