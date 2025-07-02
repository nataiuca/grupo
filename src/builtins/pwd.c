/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzolotar <mzolotar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 12:37:37 by mzolotar          #+#    #+#             */
/*   Updated: 2025/06/30 08:33:15 by mzolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "minishell.h"

char	*ft_get_env_aux(t_env *env, const char *key)
{
	if (!env)
		return NULL;
	while (env)
	{
		if (ft_strcmp(env->key, key) == 0)
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

static int	check_pwd_error(t_program *program, char **args)
{
	if (!args[1])
		return (0); // no hay error
	if (args[1][0] == '-') // opción inválida
	{
		ft_putstr_fd("bash: pwd: ", STDERR_FILENO);
		ft_putstr_fd(args[1], STDERR_FILENO); // solo la letra
		ft_putstr_fd(": invalid option", STDERR_FILENO);
		ft_putstr_fd("\n", STDERR_FILENO);
		ft_putstr_fd("Try 'pwd' with no options.\n", STDERR_FILENO); // opcional
		program->last_exit_status = 1;
		return (1);
	}
	return(0);
}

int	ft_pwd(t_all *all, t_program *program, char **args) //🚩 NORMA
{
	char	pwd_buffer[PATH_MAX];
	char	*pwd;
	int		error_code;

	error_code = check_pwd_error(program, args);
	if (error_code != 0)
		return (error_code);
	pwd = getcwd(pwd_buffer, sizeof(pwd_buffer));
	if (!pwd) // getcwd falló → intenta usar PWD del entorno como fallback
	{
		pwd = ft_get_env_aux(program->env, "PWD");
		if (!pwd)
		{
			ft_putstr_fd("minishell: pwd: ", STDERR_FILENO);
			ft_putstr_fd(strerror(errno), STDERR_FILENO);
			ft_putchar_fd('\n', STDERR_FILENO);
			return (1);
		}
	}
	if (all->exec->outfile != -1)
	{
		write(all->exec->outfile, pwd, ft_strlen(pwd));
		write(all->exec->outfile, "\n", 1);
	}
	else
	{
		write(STDOUT_FILENO, pwd, ft_strlen(pwd));
		write(STDOUT_FILENO, "\n", 1);
	}
	return (0);
}
