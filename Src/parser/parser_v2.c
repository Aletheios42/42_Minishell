/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_v2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elorente <elorente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 21:37:09 by alepinto          #+#    #+#             */
/*   Updated: 2025/06/11 20:08:46 by elorente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Inc/minishell.h"
#include <stdlib.h>
#include "../libft/libft.h"

void	free_syntax_tree(t_tree *tree)
{
	if (!tree)
		return ;
	free_syntax_tree(tree->left);
	free_syntax_tree(tree->right);
	if (tree->tokens)
		free_token_list(tree->tokens);
	free(tree);
}

// ========== PARENTHESES PARSING ==========

t_token	*find_matching_close_paren(t_token *open_paren)
{
	t_token	*current;
	int		paren_level;

	paren_level = 1;
	if (!open_paren)
		return (NULL);
	current = open_paren->next;
	while (current && paren_level > 0)
	{
		if (current->type == TOKEN_PAREN_OPEN)
			paren_level++;
		else if (current->type == TOKEN_PAREN_CLOSE)
		{
			paren_level--;
			if (paren_level == 0)
				return (current);
		}
		current = current->next;
	}
	return (NULL);
}

t_token	*extract_and_cut_inner_tokens(t_token *open, t_token *close)
{
	t_token	*inner_start;
	t_token	*inner_end;

	if (!open || !close)
		return (NULL);
	inner_start = open->next;
	if (!inner_start || inner_start == close)
		return (NULL);
	inner_end = close->prev;
	if (!inner_end)
		return (NULL);
	open->next = close;
	close->prev = open;
	inner_start->prev = NULL;
	inner_end->next = NULL;
	return (inner_start);
}

t_token	*extract_and_cut_external_redirections(t_token *close_paren)
{
	t_token	*external_start;

	if (!close_paren || !close_paren->next)
		return (NULL);
	external_start = close_paren->next;
	close_paren->next = NULL;
	external_start->prev = NULL;
	return (external_start);
}

// ========== PARSING FUNCTIONS ==========
/*
t_tree	*parse_command(t_token *tokens)
{
	if (!tokens || (tokens->type != TOKEN_WORD
			&& tokens->type != TOKEN_LITERAL_WORD))
		return (NULL);
	if (find_token_type(tokens, TOKEN_PAREN_OPEN))
		return (parse_parentheses_group(&tokens));
	return (create_tree_node(NODE_COMMAND, tokens));
}*/

t_tree	*parse_command(t_token *tokens)
{
	if (!tokens || (tokens->type != TOKEN_WORD
			&& tokens->type != TOKEN_LITERAL_WORD))
		return (NULL);
	return (create_tree_node(NODE_COMMAND, tokens));
}

