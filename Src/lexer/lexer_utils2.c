/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elorente <elorente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 22:49:21 by elorente          #+#    #+#             */
/*   Updated: 2025/06/09 20:01:47 by elorente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "../Inc/minishell.h"
#include <readline/readline.h>
#include <readline/history.h>

void	add_token_to_list(t_token **head, t_token **tail, t_token *new_token)
{
	if (!new_token)
		return ;
	if (!*head)
	{
		*head = new_token;
		*tail = new_token;
		return ;
	}
	(*tail)->next = new_token;
	new_token->prev = *tail;
	*tail = new_token;
}

void	free_token_list(t_token *head)
{
	t_token	*temp;

	while (head)
	{
		temp = head;
		head = head->next;
		free(temp->value);
		free(temp);
	}
}

t_string_builder	*create_string_builder(void)
{
	t_string_builder	*sb;

	sb = malloc(sizeof(t_string_builder));
	if (!sb)
		return (NULL);
	sb->capacity = 64;
	sb->str = malloc(sb->capacity);
	if (!sb->str)
	{
		free(sb);
		return (NULL);
	}
	sb->len = 0;
	sb->str[0] = '\0';
	return (sb);
}

int	append_char_to_builder(t_string_builder *sb, char c)
{
	char	*new_str;
	size_t	new_capacity;

	if (sb->len + 1 >= sb->capacity)
	{
		new_capacity = sb->capacity * 2;
		new_str = malloc(new_capacity);
		if (!new_str)
			return (0);
		ft_memcpy(new_str, sb->str, sb->len + 1);
		free(sb->str);
		sb->str = new_str;
		sb->capacity = new_capacity;
	}
	sb->str[sb->len++] = c;
	sb->str[sb->len] = '\0';
	return (1);
}

char	*finalize_string_builder(t_string_builder *sb)
{
	if (!sb || !sb->str)
		return (ft_strdup(""));
	return (ft_strdup(sb->str));
}
