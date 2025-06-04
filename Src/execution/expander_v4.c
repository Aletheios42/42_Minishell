/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_v4.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elorente <elorente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 18:05:02 by alepinto          #+#    #+#             */
/*   Updated: 2025/05/27 18:06:51 by elorente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Inc/minishell.h"
#include "../libft/libft.h"
#include <dirent.h>
#include <sys/stat.h>

void	remove_empty_string(char **words, int index)
{
	int	j;

	free(words[index]);
	j = index;
	while (words[j])
	{
		words[j] = words[j + 1];
		j++;
	}
}

void	replace_tabs_with_spaces(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '\t')
			str[i] = ' ';
		i++;
	}
}

char	**split_expanded_value(const char *expanded_value)
{
	char	**words;
	int		i;

	if (!expanded_value)
		return (NULL);
	words = ft_split(expanded_value, ' ');
	if (!words)
		return (NULL);
	i = 0;
	while (words[i])
	{
		if (words[i][0] == '\0')
		{
			remove_empty_string(words, i);
			continue ;
		}
		replace_tabs_with_spaces(words[i]);
		i++;
	}
	return (words);
}

void	append_token(t_token **head, t_token **tail, t_token *new_token)
{
	if (!*head)
	{
		*head = new_token;
		*tail = new_token;
	}
	else
	{
		(*tail)->next = new_token;
		new_token->prev = *tail;
		*tail = new_token;
	}
}

t_token	*create_tokens_from_words(char **words, t_token_type base_type)
{
	t_token	*head;
	t_token	*tail;
	t_token	*new_token;
	int		i;

	if (!words)
		return (NULL);
	head = NULL;
	tail = NULL;
	i = 0;
	while (words[i])
	{
		if (words[i][0] != '\0')
		{
			new_token = create_token(ft_strdup(words[i]), base_type);
			if (!new_token)
				return (free_token_list(head), NULL);
			append_token(&head, &tail, new_token);
		}
		i++;
	}
	return (head);
}
