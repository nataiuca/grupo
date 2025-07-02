/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_path_exec_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzolotar <mzolotar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 07:35:23 by mzolotar          #+#    #+#             */
/*   Updated: 2025/06/30 07:39:34 by mzolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "minishell.h"

void free_and_exit_child(t_all *all, t_program *program, int exit_status)
{
	free_child (program, all);
	exit(exit_status);
}

int	handle_access_errors(t_program *program, t_all *all, char *cmd)
{
	struct stat	info;

	(void)all;
	if (stat(cmd, &info) == -1)
	{
		ft_error(program, MSG_ERR_NO_SUCH_FILE, cmd, 127);
		return (127);
	}
	if (S_ISDIR(info.st_mode))
	{
		ft_error(program, MSG_ERR_IS_DIRECTORY, cmd, 126);
		return (126);
	}
	if (access(cmd, X_OK) == -1)
	{
		ft_error(program, MSG_ERR_PERMISSION_DENIED, cmd, 126);
		return (126);
	}
	return  (0);
}

int 	validate_executable(t_program *program,t_all *all, char *cmd_path)
{
	struct stat	sb;

	(void)all;
	if (stat(cmd_path, &sb) == 0 && S_ISDIR(sb.st_mode))
	{
		//fprintf(stderr,"\033[0;34m ⚠️ DEBUG: testeando stat(cmd_path, &sb) == 0 && S_ISDIR(sb.st_mode) en  \n validate_executable \033[0m\n");
		ft_error(program, MSG_ERR_IS_DIRECTORY, cmd_path, 126);
		return  (126);
	}
	if (access(cmd_path, X_OK) != 0)
	{
		//fprintf(stderr,"\033[0;34m ⚠️ DEBUG: testeando access(cmd_path, X_OK) != 0 en  \n validate_executable \033[0m\n");
		ft_error(program, MSG_ERR_PERMISSION_DENIED, cmd_path, 126);
		return  (126);
	}	
	return (0);
}
