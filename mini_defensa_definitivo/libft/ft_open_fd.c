/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_open_fd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzolotar <mzolotar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 07:52:07 by mzolotar          #+#    #+#             */
/*   Updated: 2025/05/24 13:24:32 by mzolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief Opens a file and returns its file descriptor.
 *
 * Attempts to open the file at the given path using the specified flags
 * and mode. If the operation fails, prints an error and returns -1.
 *
 * @param path Path to the file to be opened.
 * @param flags Flags specifying the file access mode.
 * @param mode Permissions to use if a new file is created.
 * @return File descriptor on success, -1 on failure.
 */
int	ft_open_fd(char *path, int flags, mode_t mode)
{
	int	fd;

	fd = open(path, flags, mode);
	if (fd == -1)
	{
		perror(path);
		return (-1);
	}
	return (fd);
}
