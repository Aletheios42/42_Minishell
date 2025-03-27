/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_swap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzolotar <mzolotar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 18:25:48 by mzolotar          #+#    #+#             */
/*   Updated: 2024/11/09 18:47:38 by mzolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * Swaps the values of two integers.
 *
 * This function takes two integer pointers as arguments and swaps
 * the values they point to. The value of the integer pointed to
 * by `a` is exchanged with the value of the integer pointed to by `b`.
 *
 * @param a Pointer to the first integer.
 * @param b Pointer to the second integer.
 */

void	ft_swap_int(int *a, int *b)
{
	int	aux;

	aux = *b;
	*b = *a;
	*a = aux;
}
