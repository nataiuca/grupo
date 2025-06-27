/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_structs.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natferna <natferna@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 14:49:10 by mzolotar          #+#    #+#             */
/*   Updated: 2025/06/27 02:04:51 by natferna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//--------------------- FREE EXEC ---------------------//

void	free_exec(t_exec *exec)
{
	if (!exec)
		return ;
	cleanup_exec(exec); // Reutilizar lógica
	free(exec);
}

void free_meta_struct(t_metachars *meta)
{
	if (!meta)
		return;
	free_split_strs(meta->allow_op);
	free_split_strs(meta->var);
	free_split_strs(meta->unexpected_token);
	free(meta);
}

/**
 * @brief Frees all resources related to here-docs.
 *
 * @param here Pointer to the here-doc structure to free.
 * @return void
 */
void free_here(t_here **ph)
{
    t_here  *here;
    int     i;

    if (!ph || !*ph)
        return;
    here = *ph;
    i = 0;
    while (i < HERE_DOCS_MAX)
    {
        if (here->fd_array[i] != -1)
        {
            close(here->fd_array[i]);
            here->fd_array[i] = -1;
        }
        if (here->here_name_docs[i])
        {
            unlink(here->here_name_docs[i]);
            free(here->here_name_docs[i]);
            here->here_name_docs[i] = NULL;
        }
        i++;
    }
    if (here->expanded_line_here)
    {
        free(here->expanded_line_here);
        here->expanded_line_here = NULL;
    }
    if (here->base_cwd)
    {
        free(here->base_cwd);
        here->base_cwd = NULL;
    }
    free(here);
    *ph = NULL;
}

void free_program(t_program *program)
{
	if (!program)
		return ;
		
	if (program)
    {
        //free_env
        if (program->env)
        {
            destroy_env_list(program->env); // Liberar lista enlazada de entorno
            program->env = NULL; //no es obligatorio si no lo vuelves a usar.
        }    
   
        if (program->envp_copy)  
        {
            free_split_strs(program->envp_copy); // Liberar envp duplicado por dup_env
            program->envp_copy = NULL;		    //no es obligatorio si no lo vuelves a usar.
        }
		
        //free_prompt
        if (program->prompt_hostname)
            free(program->prompt_hostname);  // seguro ahora

    }
}

void	free_all_structs(t_all *all)
{
	{
        fprintf(stderr, "DEBUG (free_all_structs): Freeing all.line: %s\n", all->line);
        free(all->line);
        all->line = NULL; // Asegúrate de ponerlo a NULL después de liberar
    }
	if (all->exec)
    {
		free_exec(all->exec);
	    all->exec = NULL;
    }
	if (all->meta)
    {
		free_meta_struct(all->meta);
	    all->meta = NULL;
    }
	if (all->here)
    {
		free_here(&all->here);
	    all->here = NULL;
    }
	if (all->tokens_head)
    {
        free_tokens_list(&all->tokens_head);
        all->tokens_head = NULL;
        all->tokens = NULL;
    }
    else if (!all->tokens_head)         //revisar este free
    {
		free_tokens_list(&all->tokens);
	    all->tokens = NULL;
        all->tokens_head = NULL;
    }
	if (all->token)
    {
		free_split_strs(all->token);
	    all->token = NULL;
    }
	all->line = NULL;
}
