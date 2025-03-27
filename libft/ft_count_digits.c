/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_count_digits.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzolotar <mzolotar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 18:21:36 by mzolotar          #+#    #+#             */
/*   Updated: 2024/11/09 18:21:38 by mzolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief  Returns the number of digits in an integer.
 *
 * @param  n The integer whose digits are to be counted.
 * Handles INT_MIN and LONG_MIN as special cases to avoid overflow.
 *
 * @return The count of digits in the integer,
 * including the '-' for negative numbers.
 */

int	ft_count_digits(int number)
{
	int	count;

	count = 0;
	if (number == 0)
		return (1);
	if (number == INT_MIN)
	{
		number = -(number / 10);
		count = 1;
	}
	if (number < 0)
	{
		count++;
		number = -number;
	}
	while (number > 0)
	{
		number /= 10;
		count++;
	}
	return (count);
}
