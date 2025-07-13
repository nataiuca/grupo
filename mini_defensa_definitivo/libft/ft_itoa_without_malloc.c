/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa_without_malloc.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzolotar <mzolotar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 12:15:02 by mzolotar          #+#    #+#             */
/*   Updated: 2025/05/26 14:25:47 by mzolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief Converts an integer to a string without dynamic memory allocation.
 *
 * Stores the integer as a string in the provided buffer if it fits.
 *
 * @param n The integer to convert.
 * @param buffer Pointer to the buffer to store the result.
 * @param buffer_size The size of the provided buffer.
 */
void	ft_itoa_without_malloc(int n, char *buffer, size_t buffer_size)
{
	size_t	size;

	if (!buffer || buffer_size < 12)
		return ;
	size = ft_count_digits(n);
	if (size + 1 > buffer_size)
		size = buffer_size - 1;
	ft_int_to_char(n, buffer, size);
}
