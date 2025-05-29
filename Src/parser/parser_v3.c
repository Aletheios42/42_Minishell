/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_v3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elorente <elorente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 23:01:14 by elorente          #+#    #+#             */
/*   Updated: 2025/05/29 23:01:14 by elorente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Inc/minishell.h"
#include <stdlib.h>
#include "../libft/libft.h"

void	remove_parentheses(t_token **tokens, t_token *open, t_token *close)
{
	if (open->prev)
		open->prev->next = close->next;
	else
		*tokens = close->next;
	if (close->next)
		close->next->prev = open->prev;
	free(open->value);
	free(open);
	free(close->value);
	free(close);
}

t_tree	*parse_parentheses_group(t_token **tokens)
{
	t_token	*open;
	t_token	*close;
	t_token	*inner;
	t_token	*external;
	t_tree	*node;

	open = find_token_type(*tokens, TOKEN_PAREN_OPEN);
	if (!open)
		return (NULL);
	close = find_matching_close_paren(open);
	if (!close)
		return (NULL);
	inner = extract_and_cut_inner_tokens(open, close);
	external = extract_and_cut_external_redirections(close);
	remove_parentheses(tokens, open, close);
	node = create_tree_node(NODE_PAREN, external);
	if (!node)
		return (free_token_list(inner), free_token_list(external), NULL);
	node->right = parse_expression(inner);
	return (node);
}

t_tree	*parse_pipeline(t_token *tokens)
{
	t_token	*pipe_op;
	t_token	*right_tokens;
	t_tree	*pipe_node;

	pipe_op = find_pipe_operator(tokens);
	if (!pipe_op)
		return (parse_command(tokens));
	right_tokens = cut_token_list_at(&tokens, pipe_op);
	pipe_node = create_tree_node(NODE_PIPE, pipe_op);
	if (!pipe_node)
		return (NULL);
	pipe_node->left = parse_command(tokens);
	pipe_node->right = parse_pipeline(right_tokens);
	return (pipe_node);
}

t_tree	parse_logical_ops(t_token *tokens)
{
	t_token			*logical_op;
	t_token			*right_tokens;
	t_tree			*logical_node;
	t_node_type		node_type;

	logical_op = find_logical_operator(tokens);
	if (!logical_op)
		return (parse_pipeline(tokens));
	right_tokens = cut_token_list_at(&tokens, logical_op);
	if (logical_op->type == TOKEN_AND)
		node_type = NODE_AND;
	else
		node_type = NODE_OR;
	logical_node = create_tree_node(node_type, logical_op);
	if (!logical_node)
		return (NULL);
	logical_node->left = parse_logical_ops(tokens);
	logical_node->right = parse_logical_ops(right_tokens);
	return (logical_node);
}

t_tree	*parse_expression(t_token *tokens)
{
	return (parse_logical_ops(tokens));
}
