/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzolotar <mzolotar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 07:21:05 by mzolotar          #+#    #+#             */
/*   Updated: 2025/06/19 08:46:08 by mzolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "minishell.h"

static int	is_valid_numeric_arg(char *arg)  //✅ 
{
	int	i;

	i = 0;
	if (!arg || arg[0] == '\0') // vacía
		return (0);

	if (arg[0] == '+' || arg[0] == '-')
		i++;

	if (arg[i] == '\0') // solo signo sin dígitos
		return (0);

	while (arg[i])
	{
		if (!ft_isdigit(arg[i]))
			return (0);
		i++;
	}
	return (1);
}

static int	ft_aux_exit(t_exec *exec, t_program *program)  //✅ 
{
	if (exec->args[1])
	{
		if (!is_valid_numeric_arg(exec->args[1]))
		{
			ft_error(program, MSG_ERR_NOT_NUMERIC, exec->args[1], 2);
			return (2); // se devolverá luego en exit
		}
		if (exec->args[2])
		{
			ft_error(program, MSG_ERR_TOO_MANY_ARGS, "exit", 1);
			return (-1); // se queda en minishell
		}
		return (ft_atoi(exec->args[1]) % 256);
	}
	return (0); // sin argumentos
}

int	ft_exit(t_all *all, t_program *program) //✅ 
{
	int	exit_code;

	if (all->exec->num_pipes == 0)
		ft_putendl_fd("exit", STDERR_FILENO);

	exit_code = ft_aux_exit(all->exec, program);

	if (exit_code == -1)
		return (program-> last_exit_status); // no salimos, ya imprimimos error con ft_error

	//fprintf(stderr,"\033[0;34m ⚠️ DEBUG: liberando all en exit \n  \033[0m\n");
	free_all_structs(all);		// ✅ salir: liberar todo
	//fprintf(stderr,"\033[0;34m ⚠️ DEBUG: liberando program en exit \n  \033[0m\n");
	free_program(program);		// ✅ salir: liberar todo
	//program->last_exit_status = 0; //NOT
	exit(exit_code);			// ✅ terminar minishell
}
