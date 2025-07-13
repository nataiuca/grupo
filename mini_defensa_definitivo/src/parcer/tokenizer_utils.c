/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzolotar <mzolotar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 15:00:15 by mzolotar          #+#    #+#             */
/*   Updated: 2025/07/06 12:10:52 by mzolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_or_set_last_exit_status(int new_status, bool set)
{
	static int	last_status;
	static bool	initialized;

	if (!initialized)
	{
		last_status = 0;
		initialized = true;
	}
	if (set)
		last_status = new_status;
	return (last_status);
}

t_tokens	*ft_find_node_n_position(t_tokens *head, int position)
{
	while (head != NULL)
	{
		if (head->position == position)
			return (head);
		head = head->next;
	}
	return (NULL);
}

bool	ft_str_is_empty_or_spaces(const char *str)
{
	if (!str)
		return (true);
	while (*str)
	{
		if (!ft_isspace(*str))
			return (false);
		str++;
	}
	return (true);
}

void	reset_meta_flags(t_metachars *meta, int flag)
{
	if (flag == 1)
	{
		meta->i_index = 0;
		meta->j_index = 0;
	}
	else if (flag == 2)
	{
		meta->in_single_quote = false;
		meta->in_double_quote = false;
	}
	else if (flag == 3)
	{
		meta->i_index = 0;
		meta->j_index = 0;
		meta->x_index = 0;
		meta->in_single_quote = false;
		meta->in_double_quote = false;
	}
}
