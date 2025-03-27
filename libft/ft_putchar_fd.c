/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putchar_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzolotar <mzolotar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 12:57:32 by mzolotar          #+#    #+#             */
/*   Updated: 2024/11/09 18:32:42 by mzolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief Writes a character to a file descriptor.
 *
 * @param c The character to write.
 * @param fd The file descriptor to write to.
 */

void	ft_putchar_fd(char c, int fd)
{
	write(fd, &c, 1);
}
