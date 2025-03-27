/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzolotar <mzolotar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 12:50:22 by mzolotar          #+#    #+#             */
/*   Updated: 2024/11/11 11:29:11 by mzolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief Sets the first n bytes of memory to zero.
 *
 * @param s Pointer to the memory block.
 * @param n Number of bytes to set to zero.
 */

void	ft_bzero(void *s, size_t n)
{
	ft_memset(s, '\0', n);
}
