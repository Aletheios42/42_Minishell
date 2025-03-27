/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzolotar <mzolotar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 12:57:16 by mzolotar          #+#    #+#             */
/*   Updated: 2024/11/09 18:30:54 by mzolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief Searches memory for a byte.
 *
 * @param s Pointer to the memory block.
 * @param c The byte value to search for.
 * @param n Number of bytes to search.
 *
 * @return Pointer to the byte found, or NULL if not found.
 */

void	*ft_memchr(const void *s, int c, size_t n)
{
	const unsigned char	*s_pointer;
	size_t				i;

	s_pointer = (const unsigned char *)s;
	i = 0;
	while (i < n)
	{
		if (s_pointer[i] == (unsigned char)c)
			return ((void *)&s_pointer[i]);
		i++;
	}
	return (NULL);
}
