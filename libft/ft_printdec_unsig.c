/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printdec_unsig.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzolotar <mzolotar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 09:13:39 by mzolotar          #+#    #+#             */
/*   Updated: 2024/11/09 19:00:31 by mzolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief Prints an unsigned decimal number to the standard output.
 *
 * This function takes an unsigned integer as input, counts the number of
 * digits in that integer, and prints the integer to the file descriptor 1
 * (standard output). It returns the total number of digits printed.
 *
 * @param number The unsigned integer to be printed.
 *
 * @return The number of digits printed.
 */

int	ft_printdec_unsig(unsigned int number)
{
	int	digit_count;

	digit_count = ft_count_digits_unsigned(number);
	ft_putnbr_unsigned_fd(number, 1);
	return (digit_count);
}
