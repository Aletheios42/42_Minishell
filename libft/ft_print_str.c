/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_str.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzolotar <mzolotar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 11:43:35 by mzolotar          #+#    #+#             */
/*   Updated: 2024/11/09 18:59:51 by mzolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief Prints a string to the standard output.
 *
 * This function takes a string as input and writes it to the
 * standard output (file descriptor 1). If the input string is
 * NULL, it prints "(null)" instead and returns 6. Otherwise,
 * it calculates the length of the string, prints it, and
 * returns the length.
 *
 * @param str The string to be printed. If NULL, "(null)" is printed.
 *
 * @return Returns the length of the string if it's not NULL,
 *         or 6 if the string is NULL.
 */

int	ft_print_str(char *str)
{
	int	i;

	i = 0;
	if (!str)
	{
		write(1, "(null)", 6);
		return (6);
	}
	while (str[i])
		i++;
	if (ft_putstr_fd_int(str, 1) == -1)
		return (-1);
	return (ft_strlen(str));
}
