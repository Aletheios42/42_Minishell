/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isascii.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzolotar <mzolotar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 12:56:29 by mzolotar          #+#    #+#             */
/*   Updated: 2024/11/09 18:29:07 by mzolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief Checks if a character is ASCII.
 *
 * @param c The character to check.
 *
 * @return 1 if the character is ASCII, 0 otherwise.
 */

int	ft_isascii(int c)
{
	return (c >= 0 && c <= 127);
}
