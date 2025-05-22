#ifndef PARSER_H
#define PARSER_H


#include "structs.h"


// Parser function prototypes
t_tree* parser(t_token *tokens);
t_tree* parse_and_or(t_token *tokens);
t_tree* parse_pipe(t_token *tokens);
t_tree* create_tree_node(t_token *token);

// Parser utilities
t_token* split_token_list(t_token *token, t_token **left_list);
t_token* search_first_occurrence_token_type(t_token *tokens, t_token_type types[], int type_count);
void free_syntax_tree(t_tree *tree);

// Printing functions
int ft_print_syntax_tree(t_tree *tree, int depth);
int ft_print_tree_indent(int depth);
int ft_print_string_array(char **array);
int ft_print_token_list(t_token *head);
const char *get_token_type_string(t_token_type type);
void print_token_list(t_token *head);

#endif
