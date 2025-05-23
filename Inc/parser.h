#ifndef PARSER_H
#define PARSER_H

#include "structs.h"

// Main parser function
t_tree* parser(t_token *tokens);

// Tree management
t_tree* create_tree_node_with_tokens(t_node_type type, t_token *tokens);
t_tree* create_operator_node(t_token *op_token);
t_tree* create_command_node(t_token *token_list);
void free_syntax_tree(t_tree *tree);

// Token utilities
t_token* find_token_type(t_token *tokens, t_token_type types[], int type_count);
t_token* split_token_list_at(t_token *split_point, t_token **left_tokens);
t_token* clone_token_list(t_token *original);

// Parsing functions
t_tree* parse_expression(t_token *tokens);
t_tree* parse_logical_ops(t_token *tokens);
t_tree* parse_pipeline(t_token *tokens);
t_tree* parse_command_sequence(t_token *tokens);

// Syntax validation
int validate_syntax(t_token *tokens);
int validate_token_sequence(t_token *tokens);
int validate_parentheses_balance(t_token *tokens);
int validate_redirections(t_token *tokens);

// Debug functions
int ft_print_syntax_tree(t_tree *tree, int depth);
int ft_print_token_list(t_token *head);
const char *get_token_type_string(t_token_type type);
void print_token_list(t_token *head);

#endif
