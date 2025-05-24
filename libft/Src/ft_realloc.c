/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_realloc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alepinto <alepinto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 20:46:41 by elorente          #+#    #+#             */
/*   Updated: 2025/05/24 20:46:41 by elorente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_realloc(void *old_ptr, size_t chunk_size, size_t num_chunks)
{
	void	*new_ptr;
	size_t	total_size;

	total_size = chunk_size * num_chunks;
	if (!old_ptr)
		return (malloc(total_size));
	new_ptr = realloc(old_ptr, total_size);
	if (!new_ptr)
		return (NULL);
	return (new_ptr);
}
