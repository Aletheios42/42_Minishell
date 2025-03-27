/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isprint.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzolotar <mzolotar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 12:56:35 by mzolotar          #+#    #+#             */
/*   Updated: 2024/11/09 18:29:47 by mzolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief Checks if a character is printable.
 *
 * @param c The character to check.
 *
 * @return 1 if the character is printable, 0 otherwise.
 */

int	ft_isprint(int c)
{
	return (c >= 32 && c <= 126);
}
