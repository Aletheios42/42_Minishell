/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzolotar <mzolotar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 12:48:52 by mzolotar          #+#    #+#             */
/*   Updated: 2024/11/09 18:30:33 by mzolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief Converts an integer to a string.
 *
 * @param n The integer to convert.
 *
 * @return Pointer to the string, or NULL if allocation fails.
 */

char	*ft_itoa(int n)
{
	char	*new_char;
	size_t	size;

	size = ft_count_digits(n);
	new_char = (char *)malloc(size + 1);
	if (!new_char)
		return (NULL);
	ft_int_to_char(n, new_char, size);
	return (new_char);
}
