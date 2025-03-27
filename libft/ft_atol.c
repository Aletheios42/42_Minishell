/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atol.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzolotar <mzolotar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 18:21:13 by mzolotar          #+#    #+#             */
/*   Updated: 2024/11/09 18:21:16 by mzolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @brief   Converts the string pointed to by nptr to a long integer.
 *
 * This function takes a string and converts it to a long integer. It handles
 * leading whitespace characters and an optional '+' or '-' sign.
 *
 * @param   nptr A pointer to the null-terminated string to be converted.
 *
 * @return  The converted long integer value. If the string does not contain
 *          any valid number, the function returns 0.
 */

#include "libft.h"

long int	ft_atol(const char *nptr)
{
	short		sign;
	long int	number;

	sign = 1;
	number = 0;
	while ((*nptr >= 9 && *nptr <= 13) || *nptr == 32)
		nptr++;
	if (*nptr == '+' || *nptr == '-')
	{
		if (*nptr == '-')
			sign = -1;
		nptr++;
	}
	while (*nptr >= '0' && *nptr <= '9')
	{
		number = number * 10 + (*nptr - '0');
		nptr++;
	}
	return (number * sign);
}
