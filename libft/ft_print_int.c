/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_int.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzolotar <mzolotar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 09:13:18 by mzolotar          #+#    #+#             */
/*   Updated: 2024/11/09 19:00:12 by mzolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief Prints an integer to the standard output.
 *
 * This function takes an integer as input, counts the number of digits
 * in the integer, and prints it to the standard output (file descriptor 1).
 * The digit count is determined by calling the helper function
 * `ft_count_digits`. The function returns the total count of digits
 * printed.
 *
 * @param number The integer to be printed.
 *
 * @return Returns the number of digits in the integer.
 */

int	ft_print_int(int number)
{
	int	digit_count;

	if (number == INT_MIN)
	{
		ft_putnbr_fd(number, 1);
		return (11);
	}
	digit_count = (int)ft_count_digits(number);
	ft_putnbr_fd(number, 1);
	return (digit_count);
}
