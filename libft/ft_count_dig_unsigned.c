/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_count_dig_unsigned.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzolotar <mzolotar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 18:21:27 by mzolotar          #+#    #+#             */
/*   Updated: 2024/11/09 18:44:10 by mzolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief Counts the digits in an unsigned integer.
 *
 * This function calculates the number of digits in a given unsigned
 * integer by repeatedly dividing it by 10.
 *
 * @param number The unsigned integer whose digits are to be counted.
 *
 * @return int The number of digits in the unsigned integer.
 */

int	ft_count_digits_unsigned(unsigned int number)
{
	int	count;

	count = 0;
	if (number == 0)
		return (1);
	while (number > 0)
	{
		number /= 10;
		count++;
	}
	return (count);
}
