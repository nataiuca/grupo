/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_swap_void.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzolotar <mzolotar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 18:50:28 by mzolotar          #+#    #+#             */
/*   Updated: 2025/05/24 13:21:32 by mzolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_swap_void(void *s1, void *s2)
{
	unsigned char	aux;

	aux = *(unsigned char *)s1;
	*(unsigned char *)s1 = *(unsigned char *)s2;
	*(unsigned char *)s2 = aux;
}
