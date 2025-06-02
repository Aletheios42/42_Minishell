/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_v2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elorente <elorente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 19:53:34 by alepinto          #+#    #+#             */
/*   Updated: 2025/05/27 18:04:24 by elorente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Inc/minishell.h"
#include "../libft/libft.h"
#include <dirent.h>
#include <sys/stat.h>

int	has_wildcard(const char *str)
{
	if (!str)
		return (0);
	while (*str)
	{
		if (*str == '*')
			return (1);
		str++;
	}
	return (0);
}

char	*extract_directory_path(const char *pattern)
{
	char	*last_slash;

	if (!pattern)
		return (ft_strdup("."));
	last_slash = ft_strrchr(pattern, '/');
	if (!last_slash)
		return (ft_strdup("."));
	if (last_slash == pattern)
		return (ft_strdup("/"));
	return (ft_substr(pattern, 0, last_slash - pattern));
}

char	*extract_filename_pattern(const char *pattern)
{
	char	*last_slash;

	if (!pattern)
		return (NULL);
	last_slash = ft_strrchr(pattern, '/');
	if (!last_slash)
		return (ft_strdup(pattern));
	return (ft_strdup(last_slash + 1));
}

int	pattern_match(const char *pattern, const char *filename)
{
	if (!pattern || !filename)
		return (0);
	if (*pattern == '\0')
		return (*filename == '\0');
	if (*pattern == '*')
	{
		while (*filename)
		{
			if (pattern_match(pattern + 1, filename))
				return (1);
			filename++;
		}
		return (pattern_match(pattern + 1, filename));
	}
	if (*pattern == *filename)
		return (pattern_match(pattern + 1, filename + 1));
	return (0);
}

int	should_include_file(const char *filename, const char *pattern)
{
	if (!filename || !pattern)
		return (0);
	if (filename[0] == '.' && pattern[0] != '.')
		return (0);
	return (pattern_match(pattern, filename));
}
