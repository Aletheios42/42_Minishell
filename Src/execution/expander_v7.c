/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_v7.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elorente <elorente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 00:22:07 by elorente          #+#    #+#             */
/*   Updated: 2025/06/05 00:22:07 by elorente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Inc/minishell.h"
#include "../libft/libft.h"
#include <dirent.h>
#include <sys/stat.h>

char	*build_path(const char *dir_path, const char *filename)
{
	char	*temp;
	char	*full;

	if (ft_strcmp(dir_path, ".") == 0)
		return (ft_strdup(filename));
	temp = ft_strjoin(dir_path, "/");
	if (!temp)
		return (NULL);
	full = ft_strjoin(temp, filename);
	free(temp);
	return (full);
}
