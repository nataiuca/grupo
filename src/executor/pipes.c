

# include "minishell.h"

void close_all_pipe_fds_in_parent(t_exec *exec)
{
	int	i;

	i = 0;
	if (!exec->pipe_fds)
		return ;
	while (i < exec->num_pipes)
	{
		if (exec->pipe_fds[i][0] != -1 && exec->pipe_fds[i][0] > 2)
				close(exec->pipe_fds[i][0]);

		if (exec->pipe_fds[i][1] != -1 && exec->pipe_fds[i][1] > 2)
				close(exec->pipe_fds[i][1]);
		i++;
	}
}

int	init_pipe(t_program *program, t_exec *exec, int i)
{
	if (pipe(exec->pipe_fds[i]) == -1) // Intentar crear el pipe
	{
		free_pipes_up_to_index(exec, i + 1); // Liberar recursos ya asignados
		ft_error(program, MSG_ERR_PIPE, NULL, 1);
		return (-1);
	}
	return (0); // Éxito
}

int	alloc_pipe_fd_array(t_program *program, t_exec *exec)
{
	exec->pipe_fds = malloc(sizeof(int *) * exec->num_pipes);
	if (!exec->pipe_fds)
	{
		ft_error(program, MSG_ERR_MALLOC, NULL, 1);
		return (-1);
	}
	return (0);
}

int	alloc_pipe_pair_at_index(t_program *program, t_exec *exec, int i)
{
	exec->pipe_fds[i] = malloc(sizeof(int) * 2);
	if (!exec->pipe_fds[i])
	{
		free_pipes_up_to_index(exec, i);
		ft_error(program, MSG_ERR_MALLOC, NULL, 1);
		return (-1);
	}
	return (0);
}

bool	init_all_pipes(t_program *program, t_all *all)
{
	int	i;

	i = 0;
	if (alloc_pipe_fd_array(program, all->exec) == -1)
        return (false);
	while (i < all->exec->num_pipes)
	{
		if (alloc_pipe_pair_at_index(program, all->exec, i) == -1)
        {
            free_pipes_up_to_index(all->exec, i); // Si allocate_single_pipe falla.
            return (false);
        }

        if (init_pipe(program, all->exec, i) == -1) // Llamar a init_pipe
        {
			 free_pipes_up_to_index(all->exec, i + 1); //Si init_pipe falla.
			 return (false);
		}
		i++;
	}
	return (true);
}

//--------------------- FREE PIPES ---------------------//

void	free_all_pipes(t_exec *exec)
{
	int	i;

	if (!exec->pipe_fds)
		return ;
	i = 0;
	while (i < exec->num_pipes)
	{
		if (exec->pipe_fds[i])
		{
			if (exec->pipe_fds[i][0] != -1)
				close(exec->pipe_fds[i][0]);
			if (exec->pipe_fds[i][1] != -1)
				close(exec->pipe_fds[i][1]);
			free(exec->pipe_fds[i]);
		}
		i++;
	}
	free(exec->pipe_fds);
	exec->pipe_fds = NULL;
}

void	free_pipes_up_to_index(t_exec *exec, int limit)
{
	int	i;

	if (!exec->pipe_fds)
		return ;
	i = 0;

	//debug_pipes(exec, "free_pipes BEFORE close", -1);		//testeo
	while (i < limit)
	{
		if (exec->pipe_fds[i])
		{
			if (exec->pipe_fds[i][0] != -1)
				close(exec->pipe_fds[i][0]);
			if (exec->pipe_fds[i][1] != -1)
				close(exec->pipe_fds[i][1]);
			free(exec->pipe_fds[i]);
		}
		i++;
	}
	free(exec->pipe_fds);
	exec->pipe_fds = NULL;
}

// Cerrar todos los extremos de pipe (se usa en los hijos tras dup2)
void	close_all_pipes(t_exec *exec)
{
	int	i = 0;

	while (i < exec->num_pipes)
	{
		if (exec->pipe_fds[i][0] > 2)
			close(exec->pipe_fds[i][0]);
		if (exec->pipe_fds[i][1] > 2)
			close(exec->pipe_fds[i][1]);
		i++;
	}
}

// Cierra los extremos que el padre ya no usará
void	close_unused_pipes_in_parent(t_exec *exec, int i)
{
	if (i > 0) // ya no se lee del pipe anterior
	{
		if (exec->pipe_fds[i - 1][0] > 2)
			close(exec->pipe_fds[i - 1][0]);
	}
	if (i < exec->num_pipes) // ya no se escribe en el pipe actual
	{
		if (exec->pipe_fds[i][1] > 2)
			close(exec->pipe_fds[i][1]);
	}
}
