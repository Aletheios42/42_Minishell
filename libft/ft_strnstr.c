/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzolotar <mzolotar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 12:58:15 by mzolotar          #+#    #+#             */
/*   Updated: 2024/11/09 18:37:57 by mzolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief Locates a substring within a string.
 *
 * @param big The string to search.
 * @param little The substring to find.
 * @param len The maximum number of characters to search.
 *
 * @return Pointer to the first occurrence, or NULL if not found.
 */

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	i;
	size_t	j;

	if (*little == '\0')
		return ((char *)big);
	if (*big == '\0' || len == 0 || ft_strlen(little) > len)
		return (NULL);
	i = 0;
	while (i <= len - ft_strlen(little))
	{
		if (big[i] == little[0])
		{
			j = 0;
			while (j < ft_strlen(little) && big[i + j] == little[j])
			{
				j++;
			}
			if (j == ft_strlen(little))
				return ((char *)&big[i]);
		}
		i++;
	}
	return (NULL);
}
