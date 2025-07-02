/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_structs.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzolotar <mzolotar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 14:49:10 by mzolotar          #+#    #+#             */
/*   Updated: 2025/07/01 13:28:02 by mzolotar         ###   ########.fr       */
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

// mode == 0 -> FREE_ALL ->  ERROR ->free all
// mode == 1 -> FREE_CHILD -> Liberar todo excepto unlink
// mode == 2 -> FREE_ONLY_HEREDOC -> Solo liberar here_doc -> no se usa en mi programa
void free_here(t_here *here, int mode)
{
    int i;
    
    if (!here)
        return;
    i=0;
    while ( i < HERE_DOCS_MAX)
    {
        if (here->fd_array[i] != -1)
            close(here->fd_array[i]);
        if (here->here_name_docs[i])
        {
            if (mode != FREE_CHILD) // SOLO UNLINK SI NO ESTAMOS EN FREE_CHILD
                unlink(here->here_name_docs[i]);
            free(here->here_name_docs[i]);
            here->here_name_docs[i] = NULL;
        }
        i++;
    }
    if (here->base_cwd)
    {
        free(here->base_cwd);
        here->base_cwd = NULL;
    }
    free(here);
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

// mode == 0 -> ERROR ->free all
// mode == 1 -> Liberar todo excepto here_doc 
// mode == 2 -> Solo liberar here_doc -> no se usa en mi programa
void	free_all_structs(t_all *all, int mode) //🚩_NORMA
{
    if (all->line) 
    {
        free(all->line);
        all->line = NULL;
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
    if (all->tokens_head) 
    {
        free_tokens_list(&all->tokens_head);
        all->tokens_head = NULL;
        all->tokens = NULL;
    }
    if (all->token) 
    {
        free_split_strs(all->token);
        all->token = NULL;
    }
	if (all->here) 
    {
		free_here(all->here, mode);
		all->here = NULL;
	}
}
