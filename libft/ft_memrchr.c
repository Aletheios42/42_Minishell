/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzolotar <mzolotar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 18:24:34 by mzolotar          #+#    #+#             */
/*   Updated: 2024/11/09 18:24:36 by mzolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief Search for the last occurrence of a byte in a memory block.
 *
 * @param s Pointer to the memory block to search.
 * @param c Byte to search for (converted to unsigned char).
 * @param n Number of bytes to search in the block.
 * @return Pointer to the last occurrence of the byte, or NULL if not found.
 */

void	*ft_memrchr(const void *s, int c, size_t n)
{
	const unsigned char	*s_pointer;

	s_pointer = (const unsigned char *)s;
	while (n > 0)
	{
		if (s_pointer[n - 1] == (unsigned char)c)
			return ((void *)(s_pointer + n - 1));
		n--;
	}
	return (NULL);
}
