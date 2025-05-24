/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strarraystr.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alepinto <alepinto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 20:34:27 by elorente          #+#    #+#             */
/*   Updated: 2025/05/24 20:34:27 by elorente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// Busca un array de patrones en una cadena y encuentra el primero
char	*ft_strarraystr(const char *str, char **patterns)
{
	char	*earliest;
	int		i;
	size_t	len;
	char	*found;

	earliest = NULL;
	i = 0;
	len = ft_strlen(str);
	if (!str || !patterns)
		return (NULL);
	while (patterns[i])
	{
		found = ft_strnstr(str, patterns[i], len);
		if (found && (!earliest || found < earliest))
			earliest = found;
		i++;
	}
	return (earliest);
}
