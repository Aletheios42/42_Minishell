/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_unsigned_fd_own.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzolotar <mzolotar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 18:50:31 by mzolotar          #+#    #+#             */
/*   Updated: 2024/09/09 19:24:07 by mzolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief Outputs an unsigned integer to the specified file descriptor.
 *
 * This function recursively prints each digit of the unsigned integer
 * by dividing it by 10 until all digits are printed.
 *
 * @param n The unsigned integer to be printed.
 * @param fd The file descriptor to which the output will be written.
 *
 * @return void This function does not return a value.
 */

void	ft_putnbr_unsigned_fd(unsigned int n, int fd)
{
	if (n > 9)
	{
		ft_putnbr_unsigned_fd(n / 10, fd);
		ft_putnbr_unsigned_fd(n % 10, fd);
	}
	else
		ft_putchar_fd(n + '0', fd);
}
