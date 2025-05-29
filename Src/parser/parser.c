/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elorente <elorente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 21:29:51 by elorente          #+#    #+#             */
/*   Updated: 2025/05/29 21:29:51 by elorente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Inc/minishell.h"
#include <stdlib.h>
#include "../libft/libft.h"

// ========== TOKEN LIST UTILITIES ==========

t_token	*find_token_type(t_token *tokens, t_token_type type)
{
	while (tokens)
	{
		if (tokens->type == type)
			return (tokens);
		tokens = tokens->next;
	}
	return (NULL);
}

t_token	*find_logical_operator(t_token *tokens)
{
	int	paren_level;

	paren_level = 0;
	while (tokens)
	{
		if (tokens->type == TOKEN_PAREN_OPEN)
			paren_level++;
		else if (tokens->type == TOKEN_PAREN_CLOSE)
			paren_level--;
		else if (paren_level == 0
			&& (tokens->type == TOKEN_AND || tokens->type == TOKEN_OR))
			return (tokens);
		tokens = tokens->next;
	}
	return (NULL);
}

t_token	*find_pipe_operator(t_token *tokens)
{
	int	paren_level;

	paren_level = 0;
	while (tokens)
	{
		if (tokens->type == TOKEN_PAREN_OPEN)
			paren_level++;
		else if (tokens->type == TOKEN_PAREN_CLOSE)
			paren_level--;
		else if (paren_level == 0 && tokens->type == TOKEN_PIPE)
			return (tokens);
		tokens = tokens->next;
	}
	return (NULL);
}

t_token	*cut_token_list_at(t_token **head, t_token *cut_point)
{
	t_token	*right_part;

	if (!cut_point)
		return (NULL);
	right_part = cut_point->next;
	cut_point->next = NULL;
	if (right_part)
		right_part->prev = NULL;
	if (cut_point->prev)
		cut_point->prev->next = NULL;
	else
		*head = NULL;
	cut_point->prev = NULL;
	return (right_part);
}

// ========== TREE NODE MANAGEMENT ==========

t_tree	*create_tree_node(t_node_type type, t_token *tokens)
{
	t_tree	*node;

	node = malloc(sizeof(t_tree));
	if (!node)
		return (NULL);
	node->type = type;
	node->tokens = tokens;
	node->left = NULL;
	node->right = NULL;
	return (node);
}
