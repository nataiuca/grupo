
# include "minishell.h"

bool	config_fd_redir(int infile, int outfile, t_program *program)
{
	if (infile > 0 && infile != STDIN_FILENO)
	{
		if (dup2(infile, STDIN_FILENO) == -1)
		{
			fprintf(stderr,"\033[0;34m ⚠️ DEBUG: infile EN  setup_fd_redirects  \033[0m\n");
			ft_error(program, strerror(errno), "dup2 infile", 1);
			return (false);
		}
		close(infile);
	}
	if (outfile != -1 && outfile != STDOUT_FILENO)
	{
		if (dup2(outfile, STDOUT_FILENO) == -1)
		{
			fprintf(stderr,"\033[0;34m ⚠️ DEBUG: outfile EN  setup_fd_redirects  \033[0m\n");
			ft_error(program, strerror(errno), "dup2 outfile", 1);
			return (false);
		}
		close(outfile);
	}
	return (true);
}

int	handle_output(t_program *program, t_exec *exec, char *file, int flags)
{
	if (exec->outfile > 2)
	{
		//fprintf(stderr,"\033[0;34m ⚠️ DEBUG: (exec->outfile > 2) \033[0m\n");
		close(exec->outfile);
	}

	exec->outfile = open(file, flags, 0644);
	if (exec->outfile == -1)
	{
		//fprintf(stderr,"\033[0;34m ⚠️ DEBUG: saliendo handle_output \n  \033[0m\n");
		ft_error(program, strerror(errno), file, 1);
		return (-1); //NOT FREES
	}
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

	exec->infile = open(file, O_RDONLY);
	if (exec->infile == -1)
	{
		//fprintf(stderr,"\033[0;34m ⚠️ DEBUG: saliendo handle_input \n  \033[0m\n");
		ft_error(program, strerror(errno), file, 1);
		return (-1);  //NOT FREES
	}

	//printf("⚠️DEBUG: saliendo handle_input\n");
	return (0);
}

int get_output_flags(int type)
{
	if (type == redir_r_u)
		return (O_WRONLY | O_CREAT | O_TRUNC);
	return (O_WRONLY | O_CREAT | O_APPEND);
}

int	process_output_redirection(t_program *program, t_exec *exec, t_tokens *curr)
{
	int	flags;
	int	result;
	
	//printf("⚠️DEBUG:entrando process_output_redirection\n");
	flags = get_output_flags(curr->type);

	if (!curr->next)
	{
		ft_error(program, "Missing redirection target", NULL, 1); 	//🚩🚩🚩revisar este error
		return (-1);
	}

	result = handle_output(program, exec, curr->next->content, flags);
	if (result == -1 || exec->outfile == -1)
		return (-1);
	//fprintf(stderr,"\033[0;31m ⚠️DEBUG: process_output_redirection -> infile %d: \033[0m\n", exec->infile);
	//fprintf(stderr,"\033[0;31m ⚠️DEBUG: process_output_redirection -> outfile %d: \033[0m\n", exec->outfile);
	//printf("⚠️DEBUG:saliendo process_output_redirection\n");
	return (0);
}

int	process_input_redirection(t_all *all, t_program *program, t_tokens *curr, int *heredoc_index)
{
	char	*file;
	int		result;
	//printf("⚠️DEBUG:entrando process_input_redirection\n");
	
	if (curr->type == redir_l_d)
	{
		file = all->here->here_name_docs[*heredoc_index];
		(*heredoc_index)++;
	}
	else
		file = curr->next->content;
		
	if (!curr->next)
	{
		ft_error(program, "Missing redirection target", NULL, 1); 	//🚩🚩🚩revisar este error
		return (-1);
	}
	result = handle_input(program, all->exec, file);
	
	if (result == -1 || all->exec->infile == -1)
	{
		//printf("⚠️DEBUG: saliendo process_input_redirection con -1\n");
		return (-1);
	}
	//printf("⚠️DEBUG: saliendo process_input_redirection\n");
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
        if (curr->type == redir_r_u || curr->type == redir_r_d)
        {
            ret = process_output_redirection(program, all->exec, curr);
            if (ret == -1)
                return (false);
        }
        else if (curr->type == redir_l_u || curr->type == redir_l_d)
        {
            ret = process_input_redirection(all, program, curr, &heredoc_index);
            if (ret == -1)
                return (false);
        }
        curr = curr->next;
    }
    return (true);
}

static bool	redirect_input(t_exec *exec, int i, t_program *program)
{
	if (exec->infile != -1)
	{
		//fprintf(stderr, "⚠️DEBUG -> REDIR: dup2 infile %d -> stdin\n", exec->infile);
		if (dup2(exec->infile, STDIN_FILENO) == -1)
			return (ft_error(program, strerror(errno), "dup2 infile", 1), false);
	}
	else if (i > 0)
	{
		//fprintf(stderr, "⚠️DEBUG -> REDIR: dup2 pipe[%d][0] (%d) -> stdin\n", i - 1, exec->pipe_fds[i - 1][0]);
		if (dup2(exec->pipe_fds[i - 1][0], STDIN_FILENO) == -1)
			return (ft_error(program, strerror(errno), "dup2 pipe infile", 1), false);
	}
	return (true);
}

static bool	redirect_output(t_exec *exec, int i, t_program *program)
{
	if (exec->outfile != -1)
	{
		//fprintf(stderr, "⚠️DEBUG -> REDIR: dup2 outfile %d -> stdout\n", exec->outfile);
		if (dup2(exec->outfile, STDOUT_FILENO) == -1)
			return (ft_error(program, strerror(errno), "dup2 outfile", 1), false);
	}
	else if (i < exec->num_pipes)
	{
		//fprintf(stderr, "⚠️DEBUG -> REDIR: dup2 pipe[%d][1] (%d) -> stdout\n", i, exec->pipe_fds[i][1]);
		if (dup2(exec->pipe_fds[i][1], STDOUT_FILENO) == -1)
			return (ft_error(program, strerror(errno), "dup2 pipe outfile", 1), false);
	}
	return (true);
}

bool	apply_cmd_redir(t_exec *exec, int i, t_program *program)
{
	int	j;

	j = 0;
	if (!redirect_input(exec, i, program))
		return (false);
	if (!redirect_output(exec, i, program))
		return (false);
	while (j < exec->num_pipes)
	{
		close(exec->pipe_fds[j][0]);
		close(exec->pipe_fds[j][1]);
		j++;
	}
	return (true);
}
