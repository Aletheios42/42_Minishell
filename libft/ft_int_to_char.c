/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_int_to_char.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzolotar <mzolotar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 18:22:30 by mzolotar          #+#    #+#             */
/*   Updated: 2024/11/09 18:22:32 by mzolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief  Converts an integer to its string representation.
 *
 * @param  n The integer to be converted.
 * @param  new_char A pointer to the allocated memory
	where the result will be stored.
 * @param  size The size of the string, including the sign and '\0'.
 *
 * @return A pointer to the resulting string.
 */

void	ft_int_to_char(int n, char *new_char, size_t size)
{
	new_char[size] = '\0';
	if (n == 0)
	{
		new_char[0] = '0';
		return ;
	}
	if (n < 0)
	{
		new_char[0] = '-';
		if (n == INT_MIN)
		{
			new_char[--size] = '8';
			n /= 10;
		}
		n = -n;
	}
	while (n > 0)
	{
		new_char[--size] = (n % 10) + '0';
		n /= 10;
	}
}
