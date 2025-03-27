/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzolotar <mzolotar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 12:57:19 by mzolotar          #+#    #+#             */
/*   Updated: 2024/11/09 18:31:14 by mzolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief Compares two memory blocks.
 *
 * @param s1 Pointer to the first block.
 * @param s2 Pointer to the second block.
 * @param n Number of bytes to compare.
 *
 * @return The difference between the first differing bytes, or 0 if equal.
 */

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	const unsigned char	*s1_pointer;
	const unsigned char	*s2_pointer;
	size_t				i;

	i = 0;
	s1_pointer = (const unsigned char *)s1;
	s2_pointer = (const unsigned char *)s2;
	while (i < n)
	{
		if (s1_pointer[i] != s2_pointer[i])
			return (s1_pointer[i] - s2_pointer[i]);
		i++;
	}
	return (0);
}
