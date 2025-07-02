#include "minishell.h"

void minishell_loop( t_program *program)
{
	t_all all;

	signal(SIGQUIT, SIG_IGN);
	//all.line = readline(get_prompt(program));
	all.line = get_line_prompt(program); 
	if (catch_ctrl_d(program, &all, all.line )) //Ctrl+D 
	{
		free(all.line);
		return;
	}
	while (all.line)
	{
		signal(SIGQUIT, SIG_IGN); // Ignoramos Ctrl+
		signal_handling(); //
		get_or_set_last_exit_status(program->last_exit_status, true);
		if (g_atomic == 130)
		{
			program->last_exit_status = 130;
			rl_replace_line("", 0);
			rl_on_new_line();
			g_atomic = 0;
			continue;
		}
		if (ft_str_is_empty_or_spaces(all.line))				//saltar si línea está vacía o sólo espacios
		{
			free(all.line);
			all.line = get_line_prompt(program); 
			//all.line = readline(get_prompt(program));
			if (catch_ctrl_d(program, &all, all.line))
			{
				free(all.line);
				break;
			}
			continue;
		}
		//add_history(all.line);
		if (!init_all_structs(&all, program) || !tokenizer(&all, program) || !init_exec_vals(program, &all))
		{ 
			handle_reset(&all, program);
			if (catch_ctrl_d(program, &all, all.line ))
			{
				free_all_structs(&all, 0);
				break;
			}
			continue;
		}
		ft_exec(&all, program);
		if (g_atomic == 130)		// ⚠️ Si heredoc fue interrumpido, abortar cicloAdd commentMore actions
		{
			program->last_exit_status = 130;
			handle_reset(&all, program);
			if (catch_ctrl_d(program, &all, all.line ))
			{
				free_all_structs(&all, 0);
				break;
			}
			g_atomic = 0;
			signal_handling();
			continue;
		}
		else
		{
			handle_reset(&all, program);
			if (catch_ctrl_d(program, &all, all.line ))
			{
				free_all_structs(&all, 0);
				break;
			}
			signal_handling(); //-> reseteo de señales 
		}
	}
}
