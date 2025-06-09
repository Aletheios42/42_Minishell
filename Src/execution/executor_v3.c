/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_v3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elorente <elorente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 16:07:22 by alepinto          #+#    #+#             */
/*   Updated: 2025/06/09 17:51:49 by elorente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Inc/minishell.h"
#include "../Inc/signals.h"
#include "../libft/libft.h"

int	fill_args_array(char **args, t_token *tokens)
{
	t_token	*current;
	t_token	*prev;
	int		i;

	i = 0;
	current = tokens;
	prev = NULL;
	while (current)
	{
		if (should_include_token(current, prev))
		{
			args[i] = ft_strdup(current->value);
			if (!args[i])
				return (0);
			i++;
		}
		prev = current;
		current = current->next;
	}
	args[i] = NULL;
	return (1);
}

char	**tokens_to_args_array(t_token *tokens)
{
	char	**args;
	int		count;

	count = count_word_tokens(tokens);
	if (count == 0)
	{
		ft_putendl_fd("minishell: command has no valid arguments", 2);
		return (NULL);
	}
	args = malloc(sizeof(char *) * (count + 1));
	if (!args)
		return (NULL);
	if (!fill_args_array(args, tokens))
	{
		ft_free_matrix(args);
		return (NULL);
	}
	return (args);
}
// ========== EXTERNAL COMMAND EXECUTION ==========

char	*check_absolute_path(const char *cmd)
{
	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, X_OK) == 0)
			return (ft_strdup(cmd));
	}
	return (NULL);
}

char	*search_in_path_dirs(const char *cmd, char **path_dirs)
{
	char	*temp;
	char	*full_path;
	int		i;

	i = 0;
	while (path_dirs[i])
	{
		temp = ft_strjoin(path_dirs[i], "/");
		if (temp)
		{
			full_path = ft_strjoin(temp, cmd);
			free(temp);
			if (full_path && access(full_path, X_OK) == 0)
				return (full_path);
			free(full_path);
		}
		i++;
	}
	return (NULL);
}

char	*find_command_in_path(const char *cmd, t_env *env)
{
	char	*path_env;
	char	**path_dirs;
	char	*result;

	if (!cmd)
		return (NULL);
	result = check_absolute_path(cmd);
	if (result)
		return (result);
	path_env = get_env_value(env, "PATH");
	if (!path_env)
		return (NULL);
	path_dirs = ft_split(path_env, ':');
	if (!path_dirs)
		return (NULL);
	result = search_in_path_dirs(cmd, path_dirs);
	ft_free_matrix(path_dirs);
	return (result);
}
