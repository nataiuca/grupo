
# include "minishell.h"

int	handle_access_errors(t_program *program, t_all *all, char *cmd)
{
	struct stat	info;
	(void)all;
	if (stat(cmd, &info) == -1)
	{
		//fprintf(stderr,"\033[0;34m ⚠️ DEBUG: testeando (stat(cmd, &info) == -1) en  handle_access_errors \033[0m\n");
		ft_error(program, MSG_ERR_NO_SUCH_FILE, cmd, 127);
		return  (127);
	}
	if (S_ISDIR(info.st_mode))
	{
		//fprintf(stderr,"\033[0;34m ⚠️ DEBUG: testeando (S_ISDIR(info.st_mode)) en  handle_access_errors \033[0m\n");
		ft_error(program, MSG_ERR_IS_DIRECTORY, cmd, 126);
		return  (126);
	}
	if (access(cmd, X_OK) == -1)
	{
		//fprintf(stderr,"\033[0;34m ⚠️ DEBUG: testeando (access(cmd, X_OK) == -1) en  handle_access_errors \033[0m\n");
		ft_error(program, MSG_ERR_PERMISSION_DENIED, cmd, 126);
		return  (126);
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
	return 0;
}

//------------------ GET PATH --------------------//

char	*get_cmd_full_path(char *cmd)
{
	char	*path;

	if (!cmd)
		return (NULL);
	if (access(cmd, F_OK) == 0) // Solo comprobamos si existe
	{
		path = ft_strdup(cmd);
		if (!path)
			return (NULL);
		return (path); // Devolvemos el path aunque no sea ejecutable
	}
	return (NULL);
}

char	*check_path(char *path, char *cmd)
{
	char	*temp;
	char	*full_path;

	temp = ft_strjoin(path, "/");
	full_path = ft_strjoin(temp, cmd);
	free(temp);
	if (!full_path)
		return (NULL);
	if (access(full_path, X_OK) == 0)
		return (full_path);
	free(full_path);
	return (NULL);
}

char	**extract_path_dirs(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
			return (ft_split(envp[i] + 5, ':'));
		i++;
	}
	return (NULL);
}


char	*find_cmd_in_paths(t_program *program, char *cmd)
{
	char	**paths;
	char	*full_path;
	int		i;

	paths = extract_path_dirs(program->envp_copy);
	full_path = NULL;
	i = 0;
	if (!paths)
		return (NULL);
	while (paths[i])
	{
		full_path = check_path(paths[i], cmd);
		if (full_path)
		{
			free_split_strs(paths);
			return (full_path);
		}
		i++;
	}
	free_split_strs(paths);
	return (NULL);
}


char	*resolve_command_path(t_tokens *curr, t_all *all, t_program *program)
{
	char	*cmd;
	char	*cmd_path;
	char	**paths;

	cmd = curr->content;
	if (!cmd || is_empty_cmd(cmd) == true)
		return (ft_error(program, MSG_ERR_CMD_NOT_FOUND, "''", 127), NULL); 
	paths = extract_path_dirs(program->envp_copy);
	if (!paths && !ft_strchr(cmd, '/'))
		return (ft_error(program, MSG_ERR_NO_SUCH_FILE, cmd, 127), NULL);
	if (paths)
		free_split_strs(paths); 
	if (cmd[0] == '/' || ft_strncmp(cmd, "./", 2) == 0 || ft_strncmp(cmd, "../", 3) == 0)
		cmd_path = get_cmd_full_path(cmd);
	else
		cmd_path = find_cmd_in_paths(program, cmd);
	if (!cmd_path)
	{
		if (cmd[0] == '/' || ft_strncmp(cmd, "./", 2) == 0 || ft_strncmp(cmd, "../", 3) == 0)
			handle_access_errors(program, all, cmd);
		else
			ft_error(program, MSG_ERR_CMD_NOT_FOUND, cmd, 127);
		return (NULL);
	}
	return (cmd_path);
}


//------------------- EXECVE ---------------------//
void free_and_exit_child(t_all *all, t_program *program, int exit_status)
{
	free_child (program, all);
	exit(exit_status);
}

void	execute_command(t_all *all, t_program *program, t_tokens *curr)
{
	//fprintf(stderr,"\033[0;34m ⚠️DEBUG: ha entrado en exec_cmd en hijo \n  \033[0m\n");
	char	*cmd_path = NULL;
	int		status;

	fprintf(stderr, "DEBUG (execute_command): Executing command:\n");
    fprintf(stderr, "  Path: %s\n", cmd_path);
    fprintf(stderr, "  Args:\n");
    for (int i = 0; all->exec->args && all->exec->args[i]; i++)
        fprintf(stderr, "    [%d]: \"%s\"\n", i, all->exec->args[i]);
	if (!curr || !curr->content || !update_args(all->exec, curr, program))
		exit(1);
	update_envp_copy(program);
	cmd_path = resolve_command_path(curr, all, program);
	if (!cmd_path)
	{
		//fprintf(stderr,"\033[0;34m ⚠️ DEBUG: saliendo de get_executable_path (cmd not found) en exec_cmd\n  \033[0m\n");
		free_and_exit_child(all, program, program->last_exit_status);
	}
	//fprintf(stderr,"\033[0;34m ⚠️DEBUG: despues de get_executable_path \n  \033[0m\n");
	status = validate_executable(program, all, cmd_path);
	if (status != 0)
	{
		free(cmd_path);
		free_and_exit_child(all, program, program->last_exit_status);
	}
	//fprintf(stderr,"\033[0;34m ⚠️DEBUG: despues de get_executable_path \n  \033[0m\n");
	//fprintf(stderr,"\033[0;34m ⚠️DEBUG: all->exec->args en execve: %s \033[0m\n", all->exec->args[0]);
	//fprintf(stderr,"\033[0;34m ⚠️DEBUG: all->exec->args en execve : %s  \033[0m\n", all->exec->args[1]);
	if (execve(cmd_path, all->exec->args, program->envp_copy) != 0)
	{
		//fprintf(stderr,"\033[0;34m⚠️ DEBUG: saliendo de execve con -1 \n  \033[0m\n");	
		ft_error(program, MSG_ERR_CMD_NOT_FOUND, all->exec->args[0], 127);
		free(cmd_path);
		free_and_exit_child(all, program, 127);
	}
	//fprintf(stderr,"\033[0;34m ⚠️ DEBUG: testeando salida de execve \n  \033[0m\n");
	free(cmd_path);
	free_and_exit_child(all, program,0);
}
