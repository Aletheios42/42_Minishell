#include "../../Inc/minishell.h"
#include <stdlib.h>
#include "../../libft/libft.h"

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
    else if (token->type == TOKEN_PAREN)
        node->type = NODE_PAREN;
    else
        node->type = NODE_COMMAND; // Default for words and other tokens
    
    node->tokens = token;
    node->left = NULL;
    node->right = NULL;
    
    return node;
}

// Check if a token is a redirection
int is_redirection_token(t_token *token)
{
    if (!token)
        return 0;
    return (token->type == TOKEN_REDIR_IN ||
            token->type == TOKEN_REDIR_OUT ||
            token->type == TOKEN_APPEND ||
            token->type == TOKEN_HEREDOC);
}

// Find redirections after a parenthesized expression
t_token *find_redirections_after_paren(t_token *tokens)
{
    t_token *current;
    t_token *redirections;
    
    if (!tokens || tokens->type != TOKEN_PAREN)
        return NULL;
    
    // Look for redirections in the next tokens
    current = tokens->next;
    redirections = NULL;
    
    while (current && is_redirection_token(current))
    {
        // Found a redirection, add it to our list
        if (!redirections)
            redirections = current;
        current = current->next;
    }
    
    return redirections;
}

// Handle a command or parenthesized expression with redirections
t_tree* parse_command(t_token *token)
{
    t_tree *node;
    t_token *content_tokens;
    t_token *redirections;
    
    if (!token)
        return NULL;
    
    // Handle parenthesized expressions
    if (token->type == TOKEN_PAREN)
    {
        // Create a node for the parenthesized expression
        node = create_tree_node(token);
        if (!node)
            return NULL;
        
        // Parse the content inside parentheses (left child)
        content_tokens = lexer(token->value);
        if (content_tokens)
            node->left = parse_and_or(content_tokens);
        
        // Check for redirections after the parentheses (right child)
        redirections = find_redirections_after_paren(token);
        if (redirections)
        {
            // Create a command node for the redirections
            node->right = create_tree_node(redirections);
        }
        
        return node;
    }
    
    // Regular command token
    return create_tree_node(token);
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
        return parse_command(tokens); // No pipe, parse as command
    
    // Split list at pipe operator
    tokens_right = split_token_list(op_token, &tokens_left);
    
    // Create tree node for pipe operator
    tree = create_tree_node(op_token);
    if (!tree)
        return NULL;
    
    // Recursively parse left and right sides
    tree->left = parse_command(tokens_left);
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
t_tree *parser(t_token *tokens)
{
    return parse_and_or(tokens);
}
