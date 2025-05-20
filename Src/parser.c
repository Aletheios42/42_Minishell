#include "../Inc/minishell.h"
#include <stdlib.h>
#include "../libft/libft.h"

// Create a tree node for a token, determining the node type from token type
t_tree* create_tree_node(t_token *token)
{
    t_tree *node = (t_tree *)malloc(sizeof(t_tree));
    if (!node)
        return NULL;
    
    // Set node type based on token type
    if (!token)
        return NULL;
    else if (token->type == TOKEN_PIPE)
        node->type = NODE_PIPE;
    else if (token->type == TOKEN_AND)
        node->type = NODE_AND;
    else if (token->type == TOKEN_OR)
        node->type = NODE_OR;
    else
        node->type = NODE_COMMAND; // Default for words and other tokens
    
    node->tokens = token;
    node->left = NULL;
    node->right = NULL;
    
    return node;
}

// Parse pipeline operators
t_tree* parse_pipe(t_token *tokens)
{
    t_tree *tree;
    t_token_type delims[] = {TOKEN_PIPE};
    t_token *tokens_left = NULL;
    t_token *op_token = NULL;
    t_token *tokens_right = NULL;
    
    if (!tokens)
        return NULL;
    
    // Find first pipe operator
    op_token = search_first_occurrence_token_type(tokens, delims, 1);
    if (!op_token)
        return create_tree_node(tokens); // No pipe, parse as command
    
    // Split list at pipe operator
    tokens_right = split_token_list(op_token, &tokens_left);
    
    // Create tree node for pipe operator
    tree = create_tree_node(op_token);
    if (!tree)
        return NULL;
    
    // Recursively parse left and right sides
    tree->left = create_tree_node(tokens_left);
    tree->right = parse_pipe(tokens_right);
    
    return tree;
}

// Parse logical operators (AND, OR)
t_tree *parse_and_or(t_token *tokens)
{
    t_tree *tree;
    t_token_type delims[] = {TOKEN_AND, TOKEN_OR};
    t_token *tokens_left = NULL;
    t_token *op_token = NULL;
    t_token *tokens_right = NULL;
    
    if (!tokens)
        return NULL;
    
    // Find first AND/OR operator
    op_token = search_first_occurrence_token_type(tokens, delims, 2);
    if (!op_token)
        return parse_pipe(tokens); // No AND/OR, parse as pipeline
    
    // Split list at AND/OR operator
    tokens_right = split_token_list(op_token, &tokens_left);
    
    // Create tree node for logical operator
    tree = create_tree_node(op_token);
    if (!tree)
        return NULL;
    
    // Recursively parse left and right sides
    tree->left = parse_and_or(tokens_left);
    tree->right = parse_and_or(tokens_right);
    
    return tree;
}

// Top-level parser function
t_tree *parser(char *input)
{
    t_token *tokens;
    tokens = lexer(input);
    
    if (!tokens)
        return NULL;
    
    return parse_and_or(tokens);
}
