/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzolotar <mzolotar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 23:46:03 by mzolotar          #+#    #+#             */
/*   Updated: 2025/06/30 12:10:28 by mzolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//--------------------- FREE EXEC ---------------------//
void cleanup_exec(t_exec *exec)
{
	int i;

	i = 0;
	if (exec->pipe_fds)
	{
		while (i < exec->num_pipes)
		{
			close(exec->pipe_fds[i][0]);
			//exec->pipe_fds[i][0] = -1;
			close(exec->pipe_fds[i][1]);
			free(exec->pipe_fds[i]);
			i++;
		}
		if (exec->pipe_fds)
		{
			free(exec->pipe_fds);
			exec->pipe_fds = NULL;
		}
	}
	if (exec->args)
	{
		free_split_strs(exec->args);		//revisar cual es el free q sirve  
		//ft_matrix_free(&exec->args);
		exec->args = NULL;
	}
	if (exec->pids) // 💥 Liberar pids
	{
		free(exec->pids);
		exec->pids = NULL;
	}
}

void	destroy_env_list(t_env *env)
{
	t_env	*tmp;

	while (env)
	{
		tmp = env->next;
		free(env->key);
		free(env->value);
		free(env);
		env = tmp;
	}
}
/*
void	clear_matrix_ptr(char ***matrix)
{
	int	i;

	i = 0;
	if (!matrix || !*matrix)
		return ;
	while ((*matrix)[i])
	{
		free((*matrix)[i]);
		(*matrix)[i++] = NULL;
	}
	free(*matrix);
	*matrix = NULL;
}
	*/
/*
void	ft_free_matrix(char **matrix)
{
	int	i;

	i = 0;
	if (!matrix)
		return ;
	while (matrix[i])
	{
		free(matrix[i]);
		i++;
	}
	free(matrix);
	matrix = NULL;
}
*/
void	destroy_env_node(t_env *node)
{
	if (!node)
		return ;
	if (node->key)
		free(node->key); // Liberar la clave
	if (node->value)
		free(node->value); // Liberar el valor
	free(node); // Liberar la estructura del nodo
}


//--------------------- FREE SPLIT---------------------//
void	free_split_strs(char **split_strs)
{
	int	i;

	if (!split_strs)
		return ;
	i = 0;
	while (split_strs[i])
	{
		free(split_strs[i]);
		split_strs[i] = NULL;
		i++;
	}
	free(split_strs);
	split_strs = NULL;
}

//--------------------- FREE TOKENS---------------------//
void free_tokens_list(t_tokens **tokens)
{
    t_tokens *current;
    t_tokens *next_node;
	//int i=0; //testeo

    if (!tokens || !*tokens)
        return;

    current = *tokens;
	//fprintf(stderr,"debug: primer nodo al entrar en free_list: %s\n",  current->content);
    while (current)
    {
		//fprintf(stderr,"debug: Liberando nodo %d con content: %s\n", i++, current->content);
		
        next_node = current->next;
        
        // Liberar el contenido de cada nodo
        if (current->content)
            free(current->content);

        // Liberar el nodo
        free(current);
        current = next_node;
    }
    *tokens = NULL;  // Asegúrate de que la lista quede vacía
}

//--------------------- FREE CHILD AND EXIT---------------------//
void	free_child (t_program *program, t_all *all)
{
	//fprintf(stderr,"\033[0;34m ⚠️ DEBUG: entrando en free_child \033[0m\n");
	//fprintf(stderr,"debug: primer nodo al entrar en free_child : %s\n",  all->tokens->content);
	//(void)all;
	if (all)
		free_all_structs(all, 1);
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
		free(program->prompt_hostname); 
		
	//fprintf(stderr,"\033[0;34m ⚠️ DEBUG: saliendo de free_child \033[0m\n");
}

