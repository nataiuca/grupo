/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_loop.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzolotar <mzolotar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 22:02:27 by mzolotar          #+#    #+#             */
/*   Updated: 2025/06/19 08:32:27 by mzolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void minishell_loop( t_program *program)
{
	t_all all;

	all.line = get_line_prompt(program);
	//all.line = readline(get_prompt(program)); 
	while (all.line)
	{
		if (ft_str_is_empty_or_spaces(all.line))				//saltar si línea está vacía o sólo espacios
		{
			free(all.line);
			all.line = get_line_prompt(program);
			//all.line = readline(get_prompt(program));
			continue;
		}
		//else 
			//add_history(all.line);
		get_or_set_last_exit_status(program->last_exit_status, true);
		if (!init_all_structs(&all, program) || !tokenizer(&all, program) || !init_exec_vals(program, &all))
		{ 
			if (!handle_reset(&all, program))
				break; // Si readline falló después del reset
			continue;
		}
		//print_test();-------------------------------------------------------------⚠️testeo------------------
		//int fd_test = ft_open_fd("dev/test/test_output/test_outputs.txt", O_RDWR | O_CREAT | O_TRUNC, 0777);
		//print_all_test_3(all.line, all.tokens, fd_test);
		//close(fd_test);
		//fin test----------------------------------------------------------------------------------------


		
		check_here_doc(&all, program, all.here);
		ft_exec(&all, program);
		if (!handle_reset(&all, program))
			break; // Si readline falló después del reset	
	}
}
