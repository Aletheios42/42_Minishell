/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_islower.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzolotar <mzolotar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 18:22:47 by mzolotar          #+#    #+#             */
/*   Updated: 2024/11/09 18:22:49 by mzolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief	Check if a character is lowercase.
 *
 * @param c	The character to check.
 *
 * @return	1 if the character is lowercase, 0 otherwise.
 */

int	ft_islower(int c)
{
	return (c >= 'a' && c <= 'z');
}
