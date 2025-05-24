/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_append_to_string_array.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alepinto <alepinto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 21:08:01 by elorente          #+#    #+#             */
/*   Updated: 2025/05/24 21:08:01 by elorente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
// Helper function to append a string to a string array
char	**ft_append_to_string_array(char ***array, char *str)
{
	char	**new_array;
	int		count;
	int		i;

	new_array = NULL;
	count = 0;
	i = 0;
	if (*array)
	{
		while ((*array)[count])
			count++;
	}
	new_array = (char **)malloc(sizeof(char *) * (count + 2));
	if (!new_array)
		return (NULL);
	while (i < count)
	{
		new_array[i] = (*array)[i];
		i++;
	}
	new_array[count] = str;
	new_array[count + 1] = NULL;
	if (*array)
		free(*array);
	return (new_array);
}
