/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   herequote_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzolotar <mzolotar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 10:42:34 by mzolotar          #+#    #+#             */
/*   Updated: 2025/06/26 10:42:35 by mzolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"

char	*herequote_hook_rl(t_program *program)
{
	char		*line;

	rl_event_hook = event_hook;
	signal(SIGINT, handler_herequote);

	(void)program;
	
	line = readline(">");
	//fprintf(stderr, "g_atomic dentro de here: %d\n ", g_atomic);
	rl_event_hook = NULL;
	return (line);
}

int	herequote_check_g_atomic(t_program *program, char *here_line)
{
	if (g_atomic)
	{
		if (here_line)
			free(here_line);
		rl_event_hook = NULL;
		program->last_exit_status = 130;
		//fprintf(stderr, "program->last_exit_status en herequote_check_g_atomic: %d\n ", program->last_exit_status);
		return (1);
	}
	return (0);
}

int	event_hook(void)
{
	if (g_atomic)
	{
		rl_done = 1;
		return (1);
	}
	return (0);
}