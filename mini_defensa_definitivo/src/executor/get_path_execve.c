/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_path_execve.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natferna <natferna@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 07:35:29 by mzolotar          #+#    #+#             */
/*   Updated: 2025/07/04 00:29:19 by natferna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_cmd_full_path(char *cmd)
{
	char	*path;

	if (!cmd)
		return (NULL);
	if (access(cmd, F_OK) == 0)
	{
		path = ft_strdup(cmd);
		if (!path)
			return (NULL);
		return (path);
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
	if (!cmd || is_empty_cmd(cmd))
		return (ft_error(program, MSG_ERR_CMD_NOT_FOUND, "''", 127), NULL);
	paths = extract_path_dirs(program->envp_copy);
	if (!paths && !ft_strchr(cmd, '/'))
		return (ft_error(program, MSG_ERR_NO_SUCH_FILE, cmd, 127), NULL);
	if (paths)
		free_split_strs(paths);
	if (is_direct_path(cmd))
		cmd_path = get_cmd_full_path(cmd);
	else
		cmd_path = find_cmd_in_paths(program, cmd);
	if (!cmd_path)
	{
		if (is_direct_path(cmd))
			handle_access_errors(program, all, cmd);
		else
			ft_error(program, MSG_ERR_CMD_NOT_FOUND, cmd, 127);
		return (NULL);
	}
	return (cmd_path);
}
