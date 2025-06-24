#include "minishell.h"

void minishell_loop(t_program *program)
{
    t_all all;

    /* Ignoramos Ctrl-\ desde el inicio del prompt */
    setup_signals(1);

    all.line = readline(get_prompt(program));
    if (catch_interactive(program, &all, all.line))  /* Ctrl-D */
    {
        free(all.line);
        return;
    }

    while (all.line)
    {
        /* Cada iteración, garantizamos el modo “padre” interactivo */
        setup_signals(1);

        get_or_set_last_exit_status(program->last_exit_status, true);

        if (g_atomic == 130)  /* Si llegamos con señal previa */
        {
            program->last_exit_status = 130;
            rl_replace_line("", 0);
            rl_on_new_line();
            g_atomic = 0;
            free(all.line);
            all.line = readline(get_prompt(program));
            continue;
        }

        if (ft_str_is_empty_or_spaces(all.line))
        {
            free(all.line);
            all.line = readline(get_prompt(program));
            if (catch_interactive(program, &all, all.line))
                break;
            continue;
        }

        add_history(all.line);

        if (!init_all_structs(&all, program)
            || !tokenizer(&all, program)
            || !init_exec_vals(program, &all))
        {
            handle_reset(&all, program);
            if (catch_interactive(program, &all, all.line))
            {
                free_all_structs(&all);
                break;
            }
            continue;
        }

        check_here_doc(&all, program, all.here);
        if (g_atomic == 130)
        {
            program->last_exit_status = 130;
            handle_reset(&all, program);
            if (catch_interactive(program, &all, all.line))
            {
                free_all_structs(&all);
                break;
            }
            g_atomic = 0;
            continue;
        }

        ft_exec(&all, program);

        free_all_structs(&all);
        all.line = readline(get_prompt(program));
        if (catch_interactive(program, &all, all.line))
        {
            free_all_structs(&all);
            break;
        }
    }
}