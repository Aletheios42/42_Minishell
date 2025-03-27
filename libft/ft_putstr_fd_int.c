/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr_fd_int.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzolotar <mzolotar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 09:57:14 by mzolotar          #+#    #+#             */
/*   Updated: 2024/11/09 18:34:04 by mzolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief Writes a string to a specified file descriptor.
 *
 * This function writes the string 's' to the file descriptor 'fd'. If the 
 * file descriptor is invalid or the string is NULL, it returns -1. Otherwise, 
 * it returns the number of characters written.
 *
 * @param s The string to write. If 's' is NULL, "(null)" will be written.
 * @param fd The file descriptor to write to. Should be a valid file 
 * descriptor.
 * @return The number of characters written, or -1 if an error occurs.
 */

int	ft_putstr_fd_int(char *s, int fd)
{
	if (!s)
		return (-1);
	if (fd == -1)
		return (-1);
	if (write(fd, s, ft_strlen(s)) == -1)
		return (-1);
	return (0);
}
