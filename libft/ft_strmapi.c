/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzolotar <mzolotar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 12:58:09 by mzolotar          #+#    #+#             */
/*   Updated: 2024/11/09 18:37:31 by mzolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief Applies a function to each character of a string, creating a new one.
 *
 * @param s The input string.
 * @param f The function to apply.
 *
 * @return A new string with the transformed characters,
 * or NULL if allocation fails.
 */

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	unsigned int	i;
	unsigned int	s_len;
	char			*new_array;

	if (!s || !f)
		return (NULL);
	s_len = (unsigned int)ft_strlen(s);
	new_array = (char *)malloc(sizeof(char) * (s_len + 1));
	if (!new_array)
		return (NULL);
	i = 0;
	while (s[i])
	{
		new_array[i] = (*f)(i, s[i]);
		i++;
	}
	new_array[i] = '\0';
	return (new_array);
}
