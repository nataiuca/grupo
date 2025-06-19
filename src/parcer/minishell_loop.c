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

void minishell_loop(t_program *program)
{
	t_all	all;
	char	*prompt;

	while (1)
	{
		// 1. Configurar señales (Ctrl+C imprime nueva línea, Ctrl+\ ignorado)
		catch_signal();

		// 2. Obtener el prompt (ej. "minishell$ ") y leer línea del usuario
		prompt = get_prompt(program);
		all.line = readline(prompt);
		// 3. Manejar Ctrl+D (EOF)
		catch_interactive(program, all.line, prompt);

		

		// 4. Saltar si la línea está vacía o solo tiene espacios
		if (ft_str_is_empty_or_spaces(all.line))
		{
			free(all.line);
			continue;
		}

		// 5. Añadir la línea al historial
		add_history(all.line);

		// 6. Guardar el último exit status antes de ejecutar
		get_or_set_last_exit_status(program->last_exit_status, true);

		// 7. Inicializar estructuras y tokenizar
		if (!init_all_structs(&all, program)
			|| !tokenizer(&all, program)
			|| !init_exec_vals(program, &all))
		{
			if (!handle_reset(&all, program))
				break; // salir del loop si hubo error grave o Ctrl+D
			continue;
		}

		// 8. Manejar heredocs
		check_here_doc(&all, program, all.here);

		// 9. Ejecutar comandos
		ft_exec(&all, program);

		// 10. Limpiar y preparar para la próxima iteración
		if (!handle_reset(&all, program))
			break;
	}
}
