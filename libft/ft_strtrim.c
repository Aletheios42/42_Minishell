/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzolotar <mzolotar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 13:37:43 by mzolotar          #+#    #+#             */
/*   Updated: 2024/11/09 18:38:38 by mzolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief Trims characters from the beginning and end of a string.
 *
 * @param s1 The string to trim.
 * @param set The set of characters to remove.
 *
 * @return A new string with trimmed characters, or NULL if allocation fails.
 */

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	start;
	size_t	end;
	size_t	new_str_len;
	char	*new_str;

	start = 0;
	end = ft_strlen(s1);
	if (!s1 || !set)
		return (NULL);
	while (s1[start] && ft_strchr(set, s1[start]))
		start++;
	while (end > start && ft_strchr(set, s1[end - 1]))
		end--;
	new_str_len = end - start;
	new_str = malloc(new_str_len + 1);
	if (!new_str)
		return (NULL);
	ft_strlcpy(new_str, s1 + start, new_str_len + 1);
	return (new_str);
}
