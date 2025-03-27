/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchar_count.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzolotar <mzolotar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 19:36:03 by mzolotar          #+#    #+#             */
/*   Updated: 2024/11/11 08:30:48 by mzolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief Counts occurrences of a character in a given string.
 *
 * This function iterates through the string `s`, counting the occurrences
 * of the character `c`. The character is cast to `unsigned char` to ensure
 * accurate matching, regardless of the signedness of `char` on the platform.
 *
 * @param s Pointer to the null-terminated string to be searched.
 *          If `s` is `NULL`, the function returns 0.
 * @param c The character to count within the string `s`.
 *          This is cast to `unsigned char` to avoid potential issues
 *          with extended ASCII or signed characters.
 *
 * @return size_t The number of occurrences of `c` in `s`. Returns 0 if
 *                `s` is `NULL` or if `c` is not found.
 */

size_t	ft_strchar_count(const char *s, int c)
{
	int		i;
	size_t	count;

	i = 0;
	count = 0;
	if (!s)
		return (0);
	while (s[i])
	{
		if ((unsigned char)s[i] == (unsigned char)c)
			count++;
		i++;
	}
	return (count);
}
