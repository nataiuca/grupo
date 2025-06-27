#include "minishell.h"

// Nueva función auxiliar para obtener la línea de entrada
char *get_input_line(t_program *program, t_all *all)
{
    char *line;
    char *prompt_str;

    if (all->needs_more_input)
        prompt_str = "> "; // Prompt de continuación
    else
        prompt_str = get_prompt(program); // Prompt normal

    line = readline(prompt_str); // Usar readline
    return (line);
}

void minishell_loop(t_program *program)
{
    t_all all;
    char *current_input_line = NULL;
    char *temp_concat_line;

	 // Añade esto al inicio del bucle:
    fprintf(stderr, "\nDEBUG (minishell_loop): New iteration. needs_more_input=%d\n", all.needs_more_input);
    if (current_input_line)
        fprintf(stderr, "  current_input_line: \"%s\"\n", current_input_line);
    else
        fprintf(stderr, "  current_input_line: NULL\n");
    // Inicializa la estructura 'all' a cero.
    // Esto es crucial para que todos los punteros dentro de 'all' sean NULL
    // al inicio, permitiendo comprobaciones seguras (ej. if (all.exec)).
    ft_bzero(&all, sizeof(t_all));
    all.needs_more_input = false; // Flag para líneas incompletas

    // Ignora SIGQUIT en el proceso principal para evitar que Ctrl+\ mate la shell.
    signal(SIGQUIT, SIG_IGN);

    current_input_line = NULL; // El puntero que "posee" la línea de entrada

    while (true) // Bucle principal de la shell
    {
        // 1. Obtener la línea de entrada
        // Si no hay línea actual o se necesita más input (ej. por comillas abiertas)
        if (!current_input_line || all.needs_more_input)
        {
            // get_input_line es una función que encapsula readline y get_line_prompt
            // y maneja la lógica de concatenación para líneas multi-línea.
            char *new_segment = get_input_line(program, &all);

            // Manejo de EOF (Ctrl+D)
            if (!new_segment)
            {
                if (current_input_line) // Si había una línea incompleta antes del EOF
                {
                    ft_putendl_fd("minishell: syntax error: unexpected end of file", STDERR_FILENO);
                    program->last_exit_status = 2;
                    free(current_input_line); // Liberar la línea incompleta
                    current_input_line = NULL;
                }
                // Si se recibe EOF y no había línea incompleta, salir del bucle.
                if (catch_interactive(program, &all, NULL))
                    break; // Sale del minishell_loop
				{
					free_all_structs(&all);
					break ;
				}
                continue; // Continúa al inicio del bucle para un nuevo prompt
            }

            // Concatenar segmentos de línea si es multi-línea
            if (current_input_line)
            {
                temp_concat_line = ft_strjoin(current_input_line, new_segment);
                free(current_input_line); // Liberar el segmento anterior
                free(new_segment);        // Liberar el nuevo segmento temporal
                current_input_line = temp_concat_line;
                if (!current_input_line) // Error de malloc durante la concatenación
                {
                    ft_error(program, MSG_ERR_MALLOC, NULL, 1);
                    break; // Sale del minishell_loop
                }
            }
            else // Primera parte de la línea o línea completa
            {
                current_input_line = new_segment;
            }
        }

        // 2. Manejo de interrupción (Ctrl+C) durante la lectura de readline
        if (g_atomic == 130)
        {
            program->last_exit_status = 130;
            if (current_input_line)
            {
                free(current_input_line); // Liberar la línea actual si existe
                current_input_line = NULL;
            }
            rl_replace_line("", 0); // Limpiar la línea de readline
            rl_on_new_line();       // Mover el cursor a una nueva línea
            g_atomic = 0;           // Resetear la bandera atómica
            all.needs_more_input = false; // Resetear flag de multi-línea
            continue;               // Volver al inicio del bucle para un nuevo prompt
        }

        // 3. Manejo de líneas vacías o solo con espacios
        if (ft_str_is_empty_or_spaces(current_input_line) && !all.needs_more_input)
        {
            free(current_input_line); // Liberar la línea vacía
            current_input_line = NULL;
            continue; // Volver al inicio del bucle para un nuevo prompt
        }

        // 4. Inicialización de estructuras (si no se necesita más input)
        if (!all.needs_more_input)
        {
            // Liberar las estructuras de la iteración anterior si existen.
            // 'free_all_structs' NO libera 'all.line' (que es 'current_input_line').
            if (all.tokens_head || all.exec || all.meta || all.here)
                free_all_structs(&all);

            // Inicializar todas las estructuras para la nueva ejecución.
            if (!init_all_structs(&all, program))
            {
                ft_error(program, MSG_ERR_MALLOC, NULL, 1);
                if (current_input_line) free(current_input_line); // Liberar la línea en caso de error fatal
                current_input_line = NULL;
                all.needs_more_input = false;
                continue; // Volver al inicio del bucle
            }
        }

        // Asignar la línea de entrada a la estructura 'all'.
        // 'all.line' es ahora un puntero que apunta a la misma memoria que 'current_input_line'.
        // 'current_input_line' sigue siendo el "dueño" de la memoria.
        all.line = current_input_line;

        // 5. Inicializar valores de ejecución (pipes, comandos, base_cwd)
        // Esto debe hacerse ANTES de tokenizer si here-docs dependen de base_cwd.
        if (!init_exec_vals(program, &all))
        {
            ft_error(program, MSG_ERR_MALLOC, NULL, 1);
            free_all_structs(&all); // Liberar lo que se haya asignado en init_all_structs
            if (current_input_line) free(current_input_line); // Liberar la línea
            current_input_line = NULL;
            all.needs_more_input = false;
            continue; // Volver al inicio del bucle
        }

        // 6. Tokenización y chequeo de sintaxis
        if (!tokenizer(&all, program))
        {
            // Si tokenizer falla, la línea actual debe ser liberada.
            // current_input_line ya se liberó si g_atomic == 130 o si es línea vacía.
            // Si no, se libera aquí.
            if (current_input_line)
            {
                free(current_input_line);
                current_input_line = NULL;
            }

            // Si el error de sintaxis indica que se necesita más input (ej. comillas abiertas)
            if (all.needs_more_input)
            {
                // No se libera 'all' completamente aquí, ya que se espera continuar
                // con la misma estructura 'all' para la siguiente parte de la línea.
                // Solo se liberan los tokens si se crearon.
                if (all.tokens_head)
                {
                    free_tokens_list(&all.tokens_head);
                    all.tokens_head = NULL;
                    all.tokens = NULL;
                }
                // Las estructuras exec, meta, here se mantienen para la siguiente iteración
                // ya que son parte del estado general de 'all'.
                continue; // Volver al inicio del bucle para pedir más input
            }
            else // Error de sintaxis que no requiere más input (ej. pipe al final)
            {
                // handle_reset libera las estructuras de 'all' (excepto all.line)
                handle_reset(&all, program);
                // current_input_line ya se puso a NULL arriba
                continue; // Volver al inicio del bucle
            }
        }

        all.needs_more_input = false; // Resetear el flag después de una tokenización exitosa

        // 7. Procesar here-docs
        // Esto debe hacerse después de la tokenización y antes de la ejecución.
        check_here_doc(&all, program, all.here);

        // Manejo de interrupción (Ctrl+C) durante la lectura de here-doc
        if (g_atomic == 130)
        {
            program->last_exit_status = 130;
            if (current_input_line) free(current_input_line); // Liberar la línea
            current_input_line = NULL;
            handle_reset(&all, program); // Liberar estructuras de 'all'
            continue; // Volver al inicio del bucle
        }

        // 8. Ejecutar el comando
        ft_exec(&all, program);

        // 9. Limpieza al final de una iteración exitosa
        // Solo liberar 'current_input_line' aquí.
        // Las estructuras 'all' se liberarán al inicio de la próxima iteración
        // o al salir del bucle principal.
        if (current_input_line)
        {
            free(current_input_line);
            current_input_line = NULL;
        }
    }

    // 10. Limpieza final al salir del minishell_loop (ej. por Ctrl+D)
    // Esto asegura que las estructuras de la última iteración se liberen.
    free_all_structs(&all);
}
