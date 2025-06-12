/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_v6.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elorente <elorente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 00:15:48 by alepinto          #+#    #+#             */
/*   Updated: 2025/06/12 16:00:33 by elorente         ###   ########.fr       */
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

void	handle_expanded_token(t_token *expanded, t_token **head, t_token **tail)
{
	t_token	*next;

	while (expanded)
	{
		next = expanded->next;
		if (expanded->value && expanded->value[0] != '\0')
		{
			expanded->next = NULL;
			add_token_to_list(head, tail, expanded);
		}
		else
		{
			free(expanded->value);
			free(expanded);
		}
		expanded = next;
	}
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
	char	*dpath;
	char	*fpattern;
	char	**matches;
	char	**full_paths;
	char	**fallback;

	if (!pattern || !has_wildcard(pattern))
		return (NULL);
	dpath = extract_directory_path(pattern);
	fpattern = extract_filename_pattern(pattern);
	if (!dpath || !fpattern)
		return (free(dpath), free(fpattern), NULL);
	matches = collect_matching_files(dpath, fpattern);
	if (!matches || !matches[0])
	{
		fallback = malloc(sizeof(char *) * 2);
		if (!fallback)
			return (free(dpath), free(fpattern), ft_free_matrix(matches), NULL);
		fallback[0] = ft_strdup(pattern);
		fallback[1] = NULL;
		return (free(dpath), free(fpattern), ft_free_matrix(matches), fallback);
	}
	sort_string_array(matches);
	full_paths = build_full_paths(dpath, matches);
	return (free(dpath), free(fpattern), ft_free_matrix(matches), full_paths);
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
