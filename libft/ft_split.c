/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzolotar <mzolotar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 09:37:33 by mzolotar          #+#    #+#             */
/*   Updated: 2025/02/09 00:36:11 by mzolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief Counts the number of words in a string separated by a delimiter.
 *
 * @param s The input string to analyze.
 * @param c The delimiter character used to split the words.
 * @return The total number of words found in the string.
 */

size_t	ft_count_words(char const *s, char c)
{
	size_t	count;
	size_t	i;

	count = 0;
	i = 0;
	while (s[i])
	{
		if (s[i] != c && (i == 0 || s[i - 1] == c))
			count++;
		i++;
	}
	return (count);
}

/**
 * @brief Frees memory allocated for an array of strings.
 *
 * @param array The array of strings to be freed.
 * @return NULL after memory has been freed.
 */

char	**ft_free(char **array)
{
	size_t	i;

	i = 0;
	if (!array)
		return (NULL);
	while (array[i])
	{
		free(array[i]);
		array[i] = NULL;
		i++;
	}
	free(array);
	return (NULL);
}

/**
 * @brief Allocates memory for each word in the array based on input string.
 *
 * @param array The array where memory for each word will be allocated.
 * @param s The input string to split into words.
 * @param c The delimiter character that separates the words.
 * @return A pointer to the array with allocated memory for each word.
 */

static char	**ft_malloc_word(char **array, char const *s, char c)
{
	int	i;
	int	j;
	int	count;

	i = 0;
	j = 0;
	count = 0;
	while (s[i])
	{
		if (s[i] != c)
			count++;
		if ((s[i] == c && i > 0 && s[i - 1] != c)
			|| (s[i] != c && s[i + 1] == '\0'))
		{
			array[j] = malloc(sizeof(char) * (count + 1));
			if (!array[j])
				return (NULL);
			count = 0;
			j++;
		}
		i++;
	}
	return (array);
}

/**
 * @brief Copies the words from the input string into the allocated array.
 *
 * @param array The array where words will be copied.
 * @param s The input string containing the words.
 * @param c The delimiter character used to split the words.
 * @return A pointer to the array with copied words.
 */

static char	**ft_cpy_word(char **array, char const *s, char c)
{
	int	i;
	int	j;
	int	x;

	i = 0;
	j = 0;
	x = 0;
	while (s[i])
	{
		if (s[i] != c)
			array[j][x++] = s[i];
		if (s[i] != c && s[i + 1] == '\0')
			array[j][x] = '\0';
		if (s[i] == c && i > 0 && s[i - 1] != c)
		{
			array[j][x] = '\0';
			j++;
			x = 0;
		}
		i++;
	}
	return (array);
}

/**
 * @brief Splits a string into an array of words based on a delimiter.
 *
 * @param s The input string to split.
 * @param c The delimiter character that separates the words.
 * @return A pointer to an array of strings (words) or NULL on failure.
 */

char	**ft_split(char const *s, char c)
{
	char	**new_array;
	size_t	words_num;

	if (!s)
		return (NULL);
	words_num = ft_count_words(s, c);
	new_array = malloc((words_num + 1) * sizeof(char *));
	if (!new_array)
		return (NULL);
	if (ft_malloc_word(new_array, s, c))
	{
		ft_cpy_word(new_array, s, c);
		new_array[words_num] = NULL;
	}
	else
		new_array = ft_free(new_array);
	return (new_array);
}
