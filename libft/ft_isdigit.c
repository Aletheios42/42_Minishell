/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isdigit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzolotar <mzolotar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 12:56:32 by mzolotar          #+#    #+#             */
/*   Updated: 2024/11/09 18:29:25 by mzolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief Checks if a character is a digit.
 *
 * @param c The character to check.
 *
 * @return 1 if the character is a digit, 0 otherwise.
 */

int	ft_isdigit(int c)
{
	return (c >= '0' && c <= '9');
}
