#include "../Inc/minishell.h"
#include <stdlib.h>

// fix:
//
// This happens because:
//
// You're not correctly breaking token lists when splitting at operators
// OR you're not handling empty token lists properly
// OR your operator-finding logic has a bug where it always finds an operator
//
// To fix this, ensure when you split tokens at an operator that:
//
// You properly set NULL terminators in the list
// You check for empty lists before recursive calls
// Add base cases to stop recursion when needed

// Find rightmost token matching any type in the array
t_token *find_rightmost_token_of_types(t_token *tokens, t_token_type types[], int num_types) {
    t_token *current = tokens;
    
    // Navigate to the end of the list first
    while (current && current->next)
        current = current->next;
    
    // Walk backward through the list
    while (current) {
        // Check if current token matches any type in the array
        for (int i = 0; i < num_types; i++) {
            if (current->type == types[i])
                return current;  // Found rightmost matching token
        }
        current = current->prev;
    }
    
    return NULL;  // No matching token found
}

// Parse a simple command with redirections
t_tree *parse_command(t_token *tokens) {
    if (!tokens)
        return NULL;
    
    // Create a command node
    t_tree *node = malloc(sizeof(t_tree));
    if (!node)
        return NULL;
        
    node->type = NODE_COMMAND;
    node->data.command.cmd = NULL;  // Will fill based on tokens
    node->data.command.infiles = NULL;
    node->data.command.outfiles = NULL;
    node->left = NULL;
    node->right = NULL;
    
    // Process tokens to fill command fields
    // (Handle redirections, commands, arguments)
    
    return node;
}

// Parse expressions with pipes
t_tree *parse_pipe(t_token *tokens) {
    if (!tokens)
        return NULL;
    
    // Find the rightmost pipe operator
    t_token_type pipe_types[] = {TOKEN_PIPE};
    t_token *pipe_token = find_rightmost_token_of_types(tokens, pipe_types, 1);
    
    if (!pipe_token) {
        // No pipe found, must be a simple command
        return parse_command(tokens);
    }
    
    // Break the token list at the pipe
    t_token *right_tokens = pipe_token->next;
    pipe_token->next = NULL;
    if (right_tokens)
        right_tokens->prev = NULL;
    
    // Create pipe node
    t_tree *node = malloc(sizeof(t_tree));
    if (!node)
        return NULL;
        
    node->type = NODE_PIPE;
    node->data.operator = "|";
    
    // Recursively process left and right sides
    node->left = parse_pipe(tokens);  // Left side might have more pipes
    node->right = parse_command(right_tokens);  // Right side is a command
    
    return node;
}

// Parse expressions with AND/OR
t_tree *parse_and_or(t_token *tokens) {
    if (!tokens)
        return NULL;
    
    // Find rightmost AND or OR operator
    t_token_type and_or_types[] = {TOKEN_AND, TOKEN_OR};
    t_token *op_token = find_rightmost_token_of_types(tokens, and_or_types, 2);
    
    if (!op_token) {
        // No AND/OR found, try pipes
        return parse_pipe(tokens);
    }
    
    // Break the token list at the operator
    t_token *right_tokens = op_token->next;
    op_token->next = NULL;
    if (right_tokens)
        right_tokens->prev = NULL;
    
    // Create operator node
    t_tree *node = malloc(sizeof(t_tree));
    if (!node)
        return NULL;
        
    node->type = (op_token->type == TOKEN_AND) ? NODE_AND : NODE_OR;
    node->data.operator = (op_token->type == TOKEN_AND) ? "&&" : "||";
    
    // Recursively process left and right sides
    node->left = parse_and_or(tokens);  // Left might have more AND/OR
    node->right = parse_and_or(right_tokens);  // Right might have more AND/OR
    
    return node;
}

// Main parser function
t_tree *parser(t_token *tokens) {
    // Start parsing at the lowest precedence level
    return parse_and_or(tokens);
}
