#include "minishell.h"

void minishell_loop( t_program *program)
{
	t_all all;

	signal(SIGQUIT, SIG_IGN);

	all.line = readline(get_prompt(program));
	//all.line = get_line_prompt(program); 
	if (catch_interactive(program, &all, all.line )) //Ctrl+D 
	{
		free(all.line);
		return;
	}

	while (all.line)
	{
		signal(SIGINT, handler);		 // Reinstalar handler cada vez por seguridad
		signal(SIGQUIT, SIG_IGN); // Ignoramos Ctrl+

		get_or_set_last_exit_status(program->last_exit_status, true);
		if (g_atomic == 130)
		{
			program->last_exit_status = 130;
			free(all.line);
			all.line = readline(get_prompt(program));
			//all.line = get_line_prompt(program); 
			if (catch_interactive(program, &all, all.line ))
			{
				//fprintf(stderr, "⛔ Saliendo del loop por Ctrl+D\n");
				free(all.line);
				break;
			}
			g_atomic = 0;
			continue;
			//fprintf(stderr, "g_atomic despues de receteo :%d\n ", g_atomic);
		}
		/*
		if (g_atomic == 130)
		{
			program->last_exit_status = 130;
			rl_replace_line("", 0);
			rl_on_new_line();
			g_atomic = 0;
			continue;
		}
			*/
			
		//write(STDERR_FILENO, "start loop\n ", 11);

		if (ft_str_is_empty_or_spaces(all.line))				//saltar si línea está vacía o sólo espacios
		{
			free(all.line);
			all.line = get_line_prompt(program); 
			//all.line = readline(get_prompt(program));
			if (catch_interactive(program, &all, all.line ))
			{
				//fprintf(stderr, "⛔ Saliendo del loop por Ctrl+D\n");
				free(all.line);
				break;
			}
			continue;
		}

		add_history(all.line);
			
		if (!init_all_structs(&all, program) || !tokenizer(&all, program) || !init_exec_vals(program, &all))
		{ 
			handle_reset(&all, program);
			if (catch_interactive(program, &all, all.line ))
			{
				//fprintf(stderr, "⛔ Saliendo del loop por Ctrl+D\n");
				free_all_structs(&all);
				break;
			}
			continue;
		}

		check_here_doc(&all, program, all.here);
		// ⚠️ Si heredoc fue interrumpido, abortar cicloAdd commentMore actions
		if (g_atomic == 130)
		{
			program->last_exit_status = 130;
			handle_reset(&all, program);
			if (catch_interactive(program, &all, all.line ))
			{
				//fprintf(stderr, "⛔ Saliendo del loop por Ctrl+D\n");
				free_all_structs(&all);
				break;
			}
			g_atomic = 0;
			continue;
			//fprintf(stderr, "g_atomic despues de receteo :%d\n ", g_atomic);
		}

		
		//print_test();-------------------------------------------------------------⚠️testeo------------------
		int fd_test = ft_open_fd("dev/test/test_output/test_outputs.txt", O_RDWR | O_CREAT | O_TRUNC, 0777);
		print_all_test_3(all.line, all.tokens, fd_test);
		close(fd_test);
		//fin test----------------------------------------------------------------------------------------

		ft_exec(&all, program);

		free_all_structs(&all);  // o handle_reset(&all, &program)
		//all.line = readline(get_prompt(program));
		all.line = get_line_prompt(program); 
		if (catch_interactive(program, &all, all.line ))
		{
			//fprintf(stderr, "⛔ Saliendo del loop por Ctrl+D\n");
			free_all_structs(&all);
			break;
		}
		signal_handling(); //revisar esto
	}
}
