/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzolotar <mzolotar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 12:01:25 by mzolotar          #+#    #+#             */
/*   Updated: 2025/07/01 13:25:49 by mzolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "minishell.h"

int	handle_output(t_program *program, t_exec *exec, char *file, int flags)
{
	//fprintf(stderr, "⚠️DEBUG entra en handle_output\n");
	if (exec->outfile > 2)
	{
		//fprintf(stderr,"\033[0;34m ⚠️ DEBUG: (exec->outfile > 2) \033[0m\n");
		close(exec->outfile);
	}

	exec->outfile = open(file, flags, 0644);
	//fprintf(stderr, "⚠️DEBUG -> REDIR: exec->outfile (%d) -> stdout\n", exec->outfile);
	if (exec->outfile == -1)
	{
		//fprintf(stderr,"\033[0;34m ⚠️ DEBUG: saliendo handle_output \n  \033[0m\n");
		ft_error(program, strerror(errno), file, 1);
		return (-1); //NOT FREES
	}
	//fprintf(stderr,"\033[0;34m ⚠️ DEBUG: return 0 en handle_output \033[0m\n");
	return (0);
}

int	handle_input(t_program *program, t_exec *exec, char *file)
{
	//printf("⚠️DEBUG: entrando handle_input\n");

	if (exec->infile > 2)
	{
		//fprintf(stderr,"\033[0;34m ⚠️ DEBUG: (exec->infile > 2) \033[0m\n");
		close(exec->infile);
	}

	//fprintf(stderr, "📌 REDIR << trying to open file: %s\n", file);
	exec->infile = open(file, O_RDONLY);
	//fprintf(stderr, "⚠️DEBUG -> REDIR: exec->infile (%d) -> stdin\n", exec->infile);
	if (exec->infile == -1)
	{
		//fprintf(stderr,"\033[0;34m ⚠️ DEBUG: saliendo handle_input \n  \033[0m\n");
		ft_error(program, strerror(errno), file, 1);
		return (-1);  //NOT FREES
	}

	printf("⚠️DEBUG: saliendo handle_input\n");
	return (0);
}

int	process_output_redirection(t_program *program, t_exec *exec, t_tokens *curr)
{
	int	flags;
	int	result;
	
	flags = get_output_flags(curr->type);
	if (!curr->next || !curr->next->content || curr->next->content[0] == '\0')
	{
		ft_error(program, "Missing redirection target", NULL, 1); 	//🚩🚩🚩revisar este error
		return (-1);
	}
	result = handle_output(program, exec, curr->next->content, flags);
	if (result == -1 || exec->outfile == -1)
		return (-1);
	return (0);
}

int	process_input_redirection(t_all *all, t_program *program, t_tokens *curr, int *heredoc_index)
{
	int result = 0;

	if (curr->type == redir_l_d) // heredoc
	{
		char *file = all->here->here_name_docs[*heredoc_index];
		if (!file)
			return (ft_error(program, "Missing heredoc temp file", NULL, 1), -1);
		if (all->exec->infile > 2)
			close(all->exec->infile);
		all->exec->infile = open(file, O_RDONLY);
		if (all->exec->infile < 0)
			return (ft_error(program, strerror(errno), file, 1), -1);
		(*heredoc_index)++;
	}
	else // redirección con nombre de archivo
	{
		if (!curr->next || !curr->next->content || curr->next->content[0] == '\0')
			return (ft_error(program, "Missing redirection target", NULL, 1), -1);
		result = handle_input(program, all->exec, curr->next->content);
		if (result == -1 || all->exec->infile == -1)
			return (-1);
	}
	return (0);
}


bool apply_redir(t_all *all, t_program *program)
{
    t_tokens *curr;
    int heredoc_index;
    int ret;

    curr = all->tokens;
    heredoc_index = 0;
    while (curr && curr->type != pipe_type)
    {
		//fprintf(stderr, "entra en while en  apply_redir con comando: %s\n", curr->content);
		//fprintf(stderr, "entra en while en  apply_redir con posicion: %d\n", curr->position);
        if (curr->type == redir_r_u || curr->type == redir_r_d)
        {
            ret = process_output_redirection(program, all->exec, curr);
            if (ret == -1 || all->exec->outfile == -1)
                return (false);
        }
        else if (curr->type == redir_l_u || curr->type == redir_l_d)
        {
            ret = process_input_redirection(all, program, curr, &heredoc_index);
            if (ret == -1 || all->exec->infile == -1)
                return (false);
        }
        curr = curr->next;
    }
	//fprintf(stderr, "sale ok del aplly_redir:\n");
	//fprintf(stderr, "⚠️DEBUG -> REDIR: exec->outfile (%d) -> stdout\n", all->exec->outfile);
	//fprintf(stderr, "⚠️DEBUG -> REDIR: exec->infile (%d) -> sstdin\n", all->exec->infile);
    return (true);
}
