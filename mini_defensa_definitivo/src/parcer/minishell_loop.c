/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_loop.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzolotar <mzolotar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 08:50:23 by mzolotar          #+#    #+#             */
/*   Updated: 2025/07/13 08:47:18 by mzolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	handle_atomic_interrupt(t_program *program)
{
	if (g_atomic == 2)
	{
		program->last_exit_status = 130;
		rl_replace_line("", 0);
		rl_on_new_line();
		g_atomic = 0;
		return (true);
	}
	return (false);
}

static int	handle_empty_input(t_program *program, t_all *all)
{
	if (ft_str_is_empty_or_spaces(all->line))
	{
		free(all->line);
		all->line = readline(get_prompt(program));
		if (catch_ctrl_d(program, all, all->line))
		{
			free(all->line);
			return (INPUT_BREAK);
		}
		return (INPUT_CONTINUE);
	}
	return (INPUT_ERROR);
}

static bool	parse_and_execute(t_program *program, t_all *all)
{
	if (!init_all_structs(all, program)
		|| !tokenizer(all, program)
		|| !init_exec_vals(program, all))
	{
		handle_reset(all, program);
		if (catch_ctrl_d(program, all, all->line))
		{
			free_all_structs(all, 0);
			return (false);
		}
		return (false);
	}
	return (true);
}

static bool	reset_and_check_exit(t_program *program, t_all *all)
{
	if (g_atomic == SIGINT || g_atomic == SIGQUIT)
	{
		program->last_exit_status = g_atomic + 128;
		handle_reset(all, program);
		if (catch_ctrl_d(program, all, all->line))
		{
			free_all_structs(all, 0);
			return (true);
		}
		g_atomic = 0;
		signal_handling();
		return (false);
	}
	if (g_atomic)
		g_atomic = 0;
	handle_reset(all, program);
	if (catch_ctrl_d(program, all, all->line))
	{
		free_all_structs(all, 0);
		return (true);
	}
	signal_handling();
	return (false);
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
		program->input_status = handle_empty_input(program, &all);
		if (program->input_status == INPUT_BREAK)
			break ;
		if (program->input_status == INPUT_CONTINUE)
			continue ;
		add_history(all.line);
		if (!parse_and_execute(program, &all))
			continue ;
		ft_exec(&all, program);
		if (reset_and_check_exit(program, &all))
			break ;
	}
}
