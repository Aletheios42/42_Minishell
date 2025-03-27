/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isspace.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzolotar <mzolotar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 18:22:59 by mzolotar          #+#    #+#             */
/*   Updated: 2024/11/09 18:23:01 by mzolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief   Checks if the given character is a whitespace character.
 *
 * This function checks if the character passed as an argument is a whitespace
 * character, which includes space, tab, newline, vertical tab,
 * form feed, and carriage return.
 *
 * @param   c The character to be checked, represented as an integer.
 *
 * @return  1 if the character is a whitespace character, 0 otherwise.
 */

int	ft_isspace(int c)
{
	return (c == 32 || (c >= 9 && c <= 13));
}
