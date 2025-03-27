/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrev.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzolotar <mzolotar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 18:53:56 by mzolotar          #+#    #+#             */
/*   Updated: 2024/11/11 08:29:04 by mzolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 *
 *
 */

char	*ft_strrev(char *str)
{
	size_t	start;
	size_t	end;

	start = 0;
	end = ft_strlen(str) - 1;
	while (start < end)
	{
		ft_swap_void(&str[start], &str[end]);
		start++;
		end--;
	}
	return (str);
}
