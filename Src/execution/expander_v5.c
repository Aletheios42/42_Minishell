/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_v5.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elorente <elorente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 00:13:04 by elorente          #+#    #+#             */
/*   Updated: 2025/06/11 21:15:51 by elorente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Inc/minishell.h"
#include "../libft/libft.h"
#include <dirent.h>
#include <sys/stat.h>

// ========== TOKEN EXPANSION WITH WILDCARD ==========
t_token	*copy_literal_token(t_token *original)
{
	char	*copy;

	copy = ft_strdup(original->value);
	if (!copy)
		return (NULL);
	return (create_token(copy, original->type));
}

t_token	*expand_assignment_token(t_token *original, t_env *env, int status)
{
	char	*expanded;

	expanded = expand_string(original->value, env, status);
	if (!expanded)
		return (NULL);
	return (create_token(expanded, original->type));
}

t_token	*handle_wildcard_token(char *expanded, t_token_type type)
{
	char	**matches;
	t_token	*tokens;

	matches = expand_wildcard_pattern(expanded);
	if (!matches)
		return (free(expanded), NULL);
	tokens = create_tokens_from_words(matches, type);
	ft_free_matrix(matches);
	return (free(expanded), tokens);
}

t_token	*split_and_create_tokens(char *expanded, t_token_type type)
{
	char	**split;
	t_token	*tokens;

	split = split_expanded_value(expanded);
	free(expanded);
	if (!split)
		return (NULL);
	tokens = create_tokens_from_words(split, type);
	return (ft_free_matrix(split), tokens);
}

t_token	*expand_and_split_token_copy(t_token *original,
		t_env *env, int exit_status)
{
	char	*expanded;

	if (!original)
		return (NULL);
	if (original->type == TOKEN_LITERAL_WORD)
		return (copy_literal_token(original));
	if (ft_strchr(original->value, '='))
		return (expand_assignment_token(original, env, exit_status));
	expanded = expand_string(original->value, env, exit_status);
	if (!*expanded)
		return (free(expanded), NULL);
	if (has_wildcard(expanded))
		return (handle_wildcard_token(expanded, original->type));
	if (!needs_splitting(expanded))
		return (create_token(expanded, original->type));
	return (split_and_create_tokens(expanded, original->type));
}
