/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_prompt.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natferna <natferna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 10:01:52 by mzolotar          #+#    #+#             */
/*   Updated: 2025/06/23 20:33:07 by natferna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Retrieves the hostname from /etc/hostname.
 *
 * Attempts to open and read the system's hostname file. If successful, it
 * returns the hostname as a string. If the file cannot be read or opened,
 * returns "unknown".
 *
 * @return A string containing the hostname or "unknown" if an error occurs.
 */
char	*get_hostname(void)
{
	int			fd;
	int			bytes_read;
	char		*newline;
	static char	hostname[HOSTNAME_SIZE];

	fd = open("/etc/hostname", O_RDONLY);
	if (fd < 0)
		return ("unknown");
	bytes_read = read(fd, hostname, HOSTNAME_SIZE - 1);
	close(fd);
	if (bytes_read <= 0)
		return ("unknown");
	hostname[bytes_read] = '\0';
	newline = ft_strchr(hostname, '\n');
	if (newline)
		*newline = '\0';
	return (hostname);
}

/**
 * @brief Builds the shell name prefix for the prompt.
 *
 * Copies the ANSI gray color code and the label "minishell: " into the
 * destination buffer, then resets the color formatting.
 *
 * @param dest Pointer to the destination buffer to hold the shell name.
 */
void	build_shell_name(char *dest)
{
	ft_strlcpy(dest, ANSI_CGRAY_DARK, PROMPT_SIZE);
	ft_strlcat(dest, "minishell: ", PROMPT_SIZE);
	ft_strlcat(dest, ANSI_RESET, PROMPT_SIZE);
}

/**
 * @brief Builds the colored user@host and path parts for the prompt.
 *
 * Constructs the user@host string in blue and the current working directory
 * in red, appending them to the respective destination buffers with ANSI
 * color codes.
 *
 * @param program Pointer to the t_program structure containing user and host.
 * @param temp_cwd Temporary string representing the working directory.
 * @param colored_userhost Output buffer for colored user@host string.
 * @param colored_path Output buffer for colored path string.
 */
void	build_colored_parts(t_program *program, char *temp_cwd,
		char *colored_userhost, char *colored_path)
{
	ft_strlcpy(colored_userhost, ANSI_BLUE, PROMPT_SIZE);
	ft_strlcat(colored_userhost, program->prompt_user, PROMPT_SIZE);
	ft_strlcat(colored_userhost, "@", PROMPT_SIZE);
	ft_strlcat(colored_userhost, program->prompt_hostname, PROMPT_SIZE);
	ft_strlcat(colored_userhost, ANSI_RESET, PROMPT_SIZE);
	ft_strlcpy(colored_path, ANSI_RED, PROMPT_SIZE);
	ft_strlcat(colored_path, ":", PROMPT_SIZE);
	ft_strlcat(colored_path, temp_cwd, PROMPT_SIZE);
	ft_strlcat(colored_path, ANSI_RESET, PROMPT_SIZE);
}

/**
 * @brief Handles getcwd failure by printing an error and returning to $HOME.
 *
 * If getcwd fails (e.g., due to a deleted current directory), this function
 * prints an error message, attempts to change the working directory to the
 * user's HOME, and updates the prompt path accordingly. If $HOME is not
 * available, it falls back to "~".
 *
 * @param program Pointer to the program structure containing shell state.
 */
void	getcwd_error(t_program *program)
{
	char	*home;

	ft_error(program, ERR_GETCWD, NULL, 1);
	home = getenv("HOME");
	if (home && chdir(home) == 0)
		ft_strlcpy(program->prompt_cwd, home, CWD_SIZE);
	else
		ft_strlcpy(program->prompt_cwd, "~", CWD_SIZE);
}

/**
 * @brief Builds and returns the complete shell prompt.
 *
 * Retrieves the current working directory and constructs the shell prompt
 * string including the shell name, user@host, and path with proper coloring.
 * Handles home directory shortening (e.g., replaces /home/user with ~).
 *
 * @param program Pointer to the t_program structure used for storing state.
 * @return A pointer to the completed shell prompt string.
 */
char	*get_prompt(t_program *program)
{
	if (!program->is_interactive)  // 🛑 Evitar prompt si no interactivo
		return (NULL);
		
	char	temp_cwd[CWD_SIZE];
	char	shell_name[PROMPT_SIZE];
	char	colored_userhost[PROMPT_SIZE];
	char	colored_path[PROMPT_SIZE];

	if (!getcwd(program->prompt_cwd, CWD_SIZE))
		getcwd_error(program);
	if (ft_strncmp(program->prompt_cwd, "/home/", 6) == 0
		&& ft_strnstr(program->prompt_cwd, program->prompt_user,
			ft_strlen(program->prompt_cwd)) != NULL)
	{
		program->prompt_home_path = program->prompt_cwd + ft_strlen("/home/")
			+ ft_strlen(program->prompt_user);
		ft_strlcpy(temp_cwd, "~", CWD_SIZE);
		ft_strlcat(temp_cwd, program->prompt_home_path, CWD_SIZE);
	}
	else
		ft_strlcpy(temp_cwd, program->prompt_cwd, CWD_SIZE);
	build_shell_name(shell_name);
	build_colored_parts(program, temp_cwd, colored_userhost, colored_path);
	ft_strlcpy(program->prompt, shell_name, PROMPT_SIZE);
	ft_strlcat(program->prompt, colored_userhost, PROMPT_SIZE);
	ft_strlcat(program->prompt, colored_path, PROMPT_SIZE);
	ft_strlcat(program->prompt, "$ ", PROMPT_SIZE);
	return (program->prompt);
}
