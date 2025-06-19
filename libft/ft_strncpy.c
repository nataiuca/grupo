/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzolotar <mzolotar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 13:20:02 by mzolotar          #+#    #+#             */
/*   Updated: 2025/05/26 14:25:54 by mzolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief Copies up to 'n' characters from 'src' to 'dest'.
 *
 * Fills remaining space in 'dest' with null bytes if 'src' is shorter than 'n'.
 *
 * @param dest Pointer to the destination buffer.
 * @param src Pointer to the source string.
 * @param n Maximum number of characters to copy.
 * @return Pointer to the destination buffer.
 */
char	*ft_strncpy(char *dest, const char *src, unsigned int n)
{
	unsigned int	i;

	i = 0;
	while (i < n && src[i] != '\0')
	{
		dest[i] = src[i];
		i++;
	}
	while (i < n)
	{
		dest[i] = '\0';
		i++;
	}
	return (dest);
}
