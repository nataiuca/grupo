/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_loop_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natferna <natferna@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 13:15:38 by natferna          #+#    #+#             */
/*   Updated: 2025/07/02 13:20:54 by natferna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	handle_atomic_interrupt(t_program *program)
{
	if (g_atomic == 130)
	{
		program->last_exit_status = 130;
		rl_replace_line("", 0);
		rl_on_new_line();
		g_atomic = 0;
		return (true);
	}
	return (false);
}
