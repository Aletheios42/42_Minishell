/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtok.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alepinto <alepinto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 20:24:34 by elorente          #+#    #+#             */
/*   Updated: 2025/05/24 20:24:34 by elorente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtok(char *str, const char *delim)
{
	static char	*last;
	char		*start;

	if (str == NULL)
		str = last;
	if (*str == '\0')
	{
		last = str;
		return (NULL);
	}
	start = str;
	while (*str && !strchr(delim, *str))
		str++;
	if (*str)
	{
		*str = '\0';
		last = str + 1;
	}
	else
		last = str;
	return (start);
}
