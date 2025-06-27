/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_structures.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natferna <natferna@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 10:33:56 by mzolotar          #+#    #+#             */
/*   Updated: 2025/06/26 21:50:28 by natferna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Initializes the main `t_program` structure, setting up initial
 * shell state, environment variables, and prompt-related information.
 *
 * @param program A pointer to the `t_program` structure to initialize.
 * @param envp The environment variables inherited from the parent process.
 * @return Void. Initializes the program structure in place.
 */
void	init_program(t_program *program, char **envp)
{
	char	*hostname_raw;

	
	if (program)
	{
		program->last_exit_status = 0;
	}
	// prompt---------------------------------------------------------------------------------------
	program->prompt_user = getenv("USER");
	if (!program->prompt_user)
		program->prompt_user = "unknown";               // no se libera,es literal
	hostname_raw = get_hostname();                     // devuelve buffer estático o literal
	program->prompt_hostname = ft_strdup(hostname_raw);// 💡 duplica siempre REVISAR DONDE SE LIBERA ESTO?
	if (!program->prompt_hostname)
		program->prompt_hostname = ft_strdup("unknown");
	if (!getcwd(program->prompt_cwd, CWD_SIZE))
		ft_strlcpy(program->prompt_cwd, "?", CWD_SIZE);
	program->prompt_home_path = NULL;
	// env:----------------------------------------------------------------------------------------
	program->envp_copy = clone_env_array(envp, program); //→ matriz (char **) copia del original para trabajar con ella
	program->env = envp_array_to_list(envp, program); //→ lista enlazada (t_env) desde envp_copy

}

/**
 * @brief Initializes and allocates memory for the `t_exec` structure,
 * which holds execution-related data like pipe FDs, arguments, and PIDs.
 *
 * @param program A pointer to the `t_program` for error handling.
 * @return A pointer to the newly initialized `t_exec` structure on success,
 * or NULL if memory allocation fails.
 */
t_exec	*init_exec(t_program *program)
{
	t_exec	*exec;

	exec = malloc(sizeof(t_exec));
	if (!exec)
	{
		//return (error_and_return(program, ERR_MALLOC, NULL, 1), NULL);
		ft_error(program, MSG_ERR_MALLOC, NULL, 1);
		return (NULL);
	}
	ft_bzero(exec, sizeof(t_exec));
	exec->pipe_fds = NULL;
	exec->args = NULL;
	exec->num_pipes = 0;
	exec->num_cmds = 0;
	exec->is_pipe = false;
	exec->pids = NULL;
	exec->infile = -1;
	exec->outfile = -1;
	return (exec);
}

/**
 * @brief Initializes and allocates memory for the `t_metachars` structure,
 * including its internal split strings for operators and unexpected tokens.
 *
 * @param program A pointer to the `t_program` for error handling.
 * @return A pointer to the newly initialized `t_metachars` structure on
 * success, or NULL if memory allocation or internal splits fail.
 */
t_metachars	*init_meta_struct(t_program *program)
{
	t_metachars	*meta;

	meta = malloc(sizeof(t_metachars));
	if (!meta)
	{
		//return (error_and_return(program, ERR_MALLOC, NULL, 1), NULL);
		ft_error(program, MSG_ERR_MALLOC, NULL, 1);
		return (NULL);
	}
	meta->program_acces = program;

	meta->cmd_seen = false;
	meta->pipe_seen = false;
	meta->expecting_cmd = false;
	meta->redir_pending = true;
	
	if (!init_meta_splits(meta, program))
		return (NULL);
	reset_meta_flags(meta, 3);
	return (meta);
}

/**
 * @brief Initializes the `t_here` structure, allocating memory for it
 * and its internal components like heredoc names.
 *
 * @param program A pointer to the `t_program` structure for error handling.
 * @return A pointer to the newly initialized `t_here` structure on success,
 * or NULL if memory allocation fails.
 */
t_here *init_here_struct(t_program *program)
{
    t_here *here;

    here = malloc(sizeof(t_here));
    if (!here)
    {
        ft_error(program, MSG_ERR_MALLOC, NULL, 1);
        return (NULL);
    }
    here->here_doc_num = 0;
    here->expanded_line_here = NULL;

    // MOVER LA ASIGNACIÓN DE base_cwd AQUÍ
    here->base_cwd = ft_strdup(program->prompt_cwd); // <-- MOVIDO AQUÍ
    if (!here->base_cwd) // Comprobar si ft_strdup falló
    {
        ft_error(program, MSG_ERR_MALLOC, NULL, 1);
        free(here); // Liberar la estructura here si la asignación de base_cwd falla
        return (NULL);
    }

    // alloc_here_doc_names ahora solo inicializa los punteros a NULL.
    // Siempre devolverá true.
    if (!alloc_here_doc_names(here, program))
    {
        // Este bloque de código ya no se ejecutará si alloc_here_doc_names
        // solo inicializa a NULL y no hace mallocs que puedan fallar.
        // Sin embargo, si en el futuro alloc_here_doc_names vuelve a hacer mallocs,
        // este bloque sería relevante de nuevo.
        free_here_doc_names(here, HERE_DOCS_MAX);
        ft_error(program, MSG_ERR_MALLOC, NULL, 1);
        free(here->base_cwd); // Liberar base_cwd si se asignó
        free(here);
        return (NULL);
    }
    return (here);
}

/**
 * @brief Initializes all core structures (`t_exec`, `t_metachars`, `t_here`)
 * required for shell operation.
 *
 * @param all A pointer to the `t_all` structure to populate.
 * @param program A pointer to the `t_program` for error handling.
 * @return True if all structures are successfully initialized, false otherwise.
 */
bool	init_all_structs(t_all *all, t_program *program)
{
	all->exec = init_exec(program);
	if (!all->exec)
	{
		return (ft_error(program, MSG_ERR_MALLOC, NULL, 1), false);
	}
	all->meta = init_meta_struct(program);
	if (!all->meta)
	{
		free_exec(all->exec); // Liberar exec antes de salir
		all->exec = NULL;
		return (ft_error(program, MSG_ERR_MALLOC, NULL, 1), false);
	}
	all->here = init_here_struct(program);
	if (!all->here)
	{
		free_exec(all->exec);
		free_meta_struct(all->meta);
		all->exec = NULL;
		all->meta = NULL;
		return (ft_error(program, MSG_ERR_MALLOC, NULL, 1), false);
	}
	
	all->tokens = NULL;
	all->tokens_head = NULL;
	all->token = NULL;
	
	return (true);
}
