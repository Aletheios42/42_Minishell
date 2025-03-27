/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzolotar <mzolotar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 12:57:29 by mzolotar          #+#    #+#             */
/*   Updated: 2024/11/09 18:32:23 by mzolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief Fills a block of memory with a byte.
 *
 * @param ptr Pointer to the memory area.
 * @param c The byte value to set.
 * @param n Number of bytes to set.
 *
 * @return Pointer to the memory area.
 */

void	*ft_memset(void *ptr, int c, size_t n)
{
	size_t			i;
	unsigned char	*p_ptr;

	p_ptr = (unsigned char *)ptr;
	i = 0;
	while (i < n)
	{
		p_ptr[i] = (unsigned char)c;
		i++;
	}
	return (ptr);
}
