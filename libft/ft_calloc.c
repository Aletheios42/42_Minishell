/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzolotar <mzolotar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 12:50:25 by mzolotar          #+#    #+#             */
/*   Updated: 2024/11/09 18:27:54 by mzolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief Allocates memory for an array and sets it to zero.
 *
 * @param nmemb Number of elements.
 * @param size Size of each element.
 *
 * @return Pointer to allocated memory, or NULL if it fails.
 */

void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*pointer;
	size_t	total_mem;

	total_mem = nmemb * size;
	pointer = malloc(total_mem);
	if (!pointer)
		return (NULL);
	ft_bzero(pointer, total_mem);
	return (pointer);
}
