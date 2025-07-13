/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzolotar <mzolotar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 20:33:46 by natferna          #+#    #+#             */
/*   Updated: 2025/07/10 10:45:32 by mzolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_output(t_program *program, t_exec *exec, char *file, int flags)
{
	if (exec->outfile > 2)
	{
		close(exec->outfile);
	}
	exec->outfile = open(file, flags, 0644);
	if (exec->outfile == -1)
	{
		ft_error(program, strerror(errno), file, 1);
		return (-1);
	}
	return (0);
}

int	handle_input(t_program *program, t_exec *exec, char *file)
{
	if (exec->infile > 2)
	{
		close(exec->infile);
	}
	exec->infile = open(file, O_RDONLY);
	if (exec->infile == -1)
	{
		ft_error(program, strerror(errno), file, 1);
		return (-1);
	}
	return (0);
}

void	close_and_exit(t_all *all, int i, t_program *program, int exit_code)
{
	close_unused_pipes(all->exec, i);
	free_and_exit_child(all, program, exit_code);
}

void	handle_check_child_result(t_all *all, t_program *program, int i)
{
	int	check_result;

	check_result = check_child_fork(program, i, all);
	if (check_result == 1)
		close_and_exit(all, i, program, program->last_exit_status);
	else if (check_result == 2)
		close_and_exit(all, i, program, 0);
}
