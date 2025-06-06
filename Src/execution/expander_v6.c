/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_v6.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alepinto <alepinto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 00:15:48 by alepinto          #+#    #+#             */
/*   Updated: 2025/06/05 00:15:48 by alepinto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Inc/minishell.h"
#include "../libft/libft.h"
#include <dirent.h>
#include <sys/stat.h>

// ========== TOKEN LIST EXPANSION - CREATES NEW LIST ==========

void	append_token_list(t_token **head, t_token **tail, t_token *new_tokens)
{
	t_token	*last;

	if (!*head)
	{
		*head = new_tokens;
		*tail = new_tokens;
	}
	else
	{
		(*tail)->next = new_tokens;
		new_tokens->prev = *tail;
	}
	last = new_tokens;
	while (last->next)
		last = last->next;
	*tail = last;
}

t_token	*expand_token_list_copy(t_token *original, t_env *env, int status)
{
	t_token	*head;
	t_token	*tail;
	t_token	*expanded;

	head = NULL;
	tail = NULL;
	while (original)
	{
		expanded = expand_and_split_token_copy(original, env, status);
		if (expanded)
			append_token_list(&head, &tail, expanded);
		original = original->next;
	}
	return (head);
}

// ========== ASSIGNMENT PROCESSING ==========

int	process_assignment(const char *assignment, t_env **env)
{
	char	*key;
	char	*value;
	char	*expanded_value;

	if (!assignment || !env || !is_assignment(assignment))
		return (0);
	if (!parse_assignment(assignment, &key, &value))
		return (0);
	expanded_value = expand_string(value, *env, 0);
	env_set_value(env, key, expanded_value);
	free(key);
	free(value);
	free(expanded_value);
	return (1);
}

char	**expand_wildcard_pattern(const char *pattern)
{
	char	*dir_path;
	char	*file_pattern;
	char	**matches;
	char	**full_paths;

	if (!pattern || !has_wildcard(pattern))
		return (NULL);
	dir_path = extract_directory_path(pattern);
	file_pattern = extract_filename_pattern(pattern);
	if (!dir_path || !file_pattern)
		return (free(dir_path), free(file_pattern), NULL);
	matches = collect_matching_files(dir_path, file_pattern);
	if (!matches || !matches[0])
	{
		free(dir_path);
		free(file_pattern);
		ft_free_matrix(matches);
		return (NULL);
	}
	sort_string_array(matches);
	full_paths = build_full_paths(dir_path, matches);
	free(dir_path);
	free(file_pattern);
	ft_free_matrix(matches);
	return (full_paths);
}

// ========== TOKEN SPLITTING ==========

int	needs_splitting(const char *str)
{
	if (!str)
		return (0);
	while (*str)
	{
		if (*str == ' ' || *str == '\t')
			return (1);
		str++;
	}
	return (0);
}
