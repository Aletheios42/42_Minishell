/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzolotar <mzolotar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 12:58:18 by mzolotar          #+#    #+#             */
/*   Updated: 2024/11/09 18:38:26 by mzolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief Locates the last occurrence of a character in a string.
 *
 * @param s The string to search.
 * @param c The character to locate.
 *
 * @return Pointer to the last occurrence, or NULL if not found.
 */

char	*ft_strrchr(const char *s, int c)
{
	int		i;
	char	*search_var;

	search_var = NULL;
	i = 0;
	while (s[i])
	{
		if (s[i] == (unsigned char)c)
			search_var = ((char *)&s[i]);
		i++;
	}
	if (s[i] == (unsigned char)c)
		return ((char *)&s[i]);
	return (search_var);
}
