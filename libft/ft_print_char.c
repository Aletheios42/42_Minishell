/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_char.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzolotar <mzolotar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 09:12:48 by mzolotar          #+#    #+#             */
/*   Updated: 2024/11/09 18:59:29 by mzolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief Prints a character to the standard output.
 *
 * This function takes a character as input and writes it to the
 * standard output (file descriptor 1) using the helper function
 * `ft_putchar_fd`. It always returns 1, indicating that one
 * character has been printed successfully.
 *
 * @param c The character to be printed.
 *
 * @return Returns 1 to indicate that one character has been printed.
 */

int	ft_print_char(char c)
{
	write(1, &c, 1);
	return (1);
}
