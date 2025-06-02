/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alepinto <alepinto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 19:36:04 by elorente          #+#    #+#             */
/*   Updated: 2025/06/02 19:36:04 by elorente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "structs.h"

// Main parser function
t_tree		*parser(t_token *tokens);

// Tree management
t_tree		*create_tree_node(t_node_type type, t_token *tokens);
void		free_syntax_tree(t_tree *tree);

// Token utilities
t_token		*find_token_type(t_token *tokens, t_token_type type);
t_token		*find_logical_operator(t_token *tokens);
t_token		*find_pipe_operator(t_token *tokens);
t_token		*find_matching_close_paren(t_token *open_paren);
t_token		*extract_and_cut_inner_tokens(t_token *open, t_token *close);
t_token		*extract_and_cut_external_redirections(t_token *close_paren);
t_token		*cut_token_list_at(t_token **head, t_token *cut_point);
void		remove_parentheses(t_token **tokens, t_token *open, t_token *close);

// Parsing functions
t_tree		*parse_expression(t_token *tokens);
t_tree		*parse_logical_ops(t_token *tokens);
t_tree		*parse_pipeline(t_token *tokens);
t_tree		*parse_parentheses_group(t_token **tokens);
t_tree		*parse_command(t_token *tokens);

// Syntax validation
int			validate_syntax(t_token *tokens);
int			validate_parentheses(t_token *tokens);
int			validate_operators(t_token *tokens);
int			validate_redirections(t_token *tokens);

// Debug functions
int			ft_print_syntax_tree(t_tree *tree, int depth);
int			print_node_id(char *prefix);
int			ft_print_token_list(t_token *head);
int			ft_print_string_array(char **array);
int			ft_print_tree_node(t_tree *tree, int dth, int lst, char *pre);
const char	*get_token_type_string(t_token_type type);
void		print_token_list(t_token *head);
void		print_tree_lines(int depth, int is_last);
int			p_operator(char	*label, t_token *token);
int			print_tree_type_info(t_tree *tree);

#endif
