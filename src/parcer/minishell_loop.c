/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_loop.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natferna <natferna@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 13:19:33 by natferna          #+#    #+#             */
/*   Updated: 2025/07/02 13:19:55 by natferna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	handle_empty_input(t_program *program, t_all *all)
{
	if (!ft_str_is_empty_or_spaces(all->line))
		return (false);
	free(all->line);
	all->line = readline(get_prompt(program));
	if (catch_ctrl_d(program, all, all->line))
	{
		free(all->line);
		return (true);
	}
	return (false);
}

static bool	parse_and_execute(t_program *program, t_all *all)
{
	if (!init_all_structs(all, program) || !tokenizer(all, program)
		|| !init_exec_vals(program, all))
		return (false);
	ft_exec(all, program);
	return (true);
}

static bool	handle_ctrl_d_and_free(t_program *program, t_all *all)
{
	if (catch_ctrl_d(program, all, all->line))
	{
		free_all_structs(all, 0);
		return (true);
	}
	return (false);
}

static bool	reset_and_check_exit(t_program *program, t_all *all)
{
	handle_reset(all, program);
	return (handle_ctrl_d_and_free(program, all));
}

void	minishell_loop(t_program *program)
{
	t_all	all;

	signal(SIGQUIT, SIG_IGN);
	all.line = readline(get_prompt(program));
	if (catch_ctrl_d(program, &all, all.line))
		return (free(all.line), (void)0);
	while (all.line)
	{
		signal(SIGQUIT, SIG_IGN);
		signal_handling();
		get_or_set_last_exit_status(program->last_exit_status, true);
		if (handle_atomic_interrupt(program))
			continue ;
		if (handle_empty_input(program, &all))
			break ;
		if (!parse_and_execute(program, &all))
		{
			if (reset_and_check_exit(program, &all))
				return ;
			continue ;
		}
		if (reset_and_check_exit(program, &all))
			return ;
		signal_handling();
	}
}
