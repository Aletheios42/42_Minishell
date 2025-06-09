/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_v3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elorente <elorente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 16:40:02 by elorente          #+#    #+#             */
/*   Updated: 2025/06/09 17:06:34 by elorente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Inc/minishell.h"
#include "../libft/libft.h"
#include <dirent.h>
#include <sys/stat.h>

// ========== WILDCARD EXPANSION ==========

int	handle_match(char ***matches, int *count, int *capacity, char *name)
{
	char	*dup;

	if (*count >= *capacity - 1)
	{
		*capacity *= 2;
		*matches = ft_realloc(*matches, sizeof(char *), *capacity);
		if (!*matches)
			return (0);
	}
	dup = ft_strdup(name);
	if (!dup)
	{
		ft_free_matrix(*matches);
		return (0);
	}
	(*matches)[*count] = dup;
	(*count)++;
	return (1);
}

char	**collect_matching_files(const char *dir_path, const char *file_pattern)
{
	DIR				*dir;
	struct dirent	*entry;
	char			**matches;
	int				count;
	int				capacity;

	dir = opendir(dir_path);
	if (!dir)
		return (NULL);
	matches = malloc(sizeof(char *) * 64);
	if (!matches)
		return (closedir(dir), NULL);
	count = 0;
	capacity = 64;
	entry = readdir(dir);
	while (entry)
	{
		if (should_include_file(entry->d_name, file_pattern))
		{
			if (!handle_match(&matches, &count, &capacity, entry->d_name))
				return (NULL);
		}
		entry = readdir(dir);
	}
	return (closedir(dir), matches[count] = NULL, matches);
}

void	swap_if_needed(char **array, int a, int b)
{
	char	*temp;

	if (ft_strcmp(array[a], array[b]) > 0)
	{
		temp = array[a];
		array[a] = array[b];
		array[b] = temp;
	}
}

void	sort_string_array(char **array)
{
	int		i;
	int		j;
	int		len;

	if (!array)
		return ;
	len = 0;
	while (array[len])
		len++;
	i = 0;
	while (i < len - 1)
	{
		j = 0;
		while (j < len - 1 - i)
		{
			swap_if_needed(array, j, j + 1);
			j++;
		}
		i++;
	}
}

char	**build_full_paths(const char *dir_path, char **filenames)
{
	char	**full_paths;
	int		count;
	int		i;

	if (!filenames)
		return (NULL);
	count = 0;
	while (filenames[count])
		count++;
	full_paths = malloc(sizeof(char *) * (count + 1));
	if (!full_paths)
		return (NULL);
	i = 0;
	while (i < count)
	{
		full_paths[i] = build_path(dir_path, filenames[i]);
		if (!full_paths[i])
			return (ft_free_matrix(full_paths), NULL);
		i++;
	}
	full_paths[count] = NULL;
	return (full_paths);
}
