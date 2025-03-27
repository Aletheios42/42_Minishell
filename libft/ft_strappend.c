/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strappend.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzolotar <mzolotar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 19:27:48 by mzolotar          #+#    #+#             */
/*   Updated: 2024/11/11 08:27:21 by mzolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief Joins two strings into a newly allocated string.
 * If 'line' is NULL, it will be treated as an empty string.
 * The function frees 'line' after appending 'temp'.
 *
 * @param line The first string (can be NULL),
	will be freed after the operation.
 * @param temp The second string to append.
 * @return Pointer to the newly allocated concatenated string.
 */

char	*ft_strappend(char *line, char *temp)
{
	size_t	i;
	size_t	j;
	char	*new_str;

	if (!line)
	{
		line = (char *)malloc(1 * sizeof(char));
		if (!line)
			return (NULL);
		line[0] = '\0';
	}
	new_str = malloc((ft_strlen(line) + ft_strlen(temp) + 1) * sizeof(char));
	if (!new_str)
		return (NULL);
	i = -1;
	j = 0;
	if (line)
		while (line[++i] != '\0')
			new_str[i] = line[i];
	while (temp[j] != '\0')
		new_str[i++] = temp[j++];
	new_str[ft_strlen(line) + ft_strlen(temp)] = '\0';
	free(line);
	return (new_str);
}
