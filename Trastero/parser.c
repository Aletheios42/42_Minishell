#include "../Inc/minishell.h"
#include <stdlib.h>
#include "../libft/libft.h"



// Process redirection token and extract filename
char **handle_redirection(char ***files, t_token **token) {
    char *filename = NULL;
    
    if ((*token)->value[1] == '\0' && (*token)->next && (*token)->next->type == TOKEN_WORD) {
        // Case: "< infile" (separate tokens)
        filename = (*token)->next->value;
        *token = (*token)->next; // Skip the filename token
    } else {
        // Case: "<infile" (combined in one token)
        filename = (*token)->value + 1; // Skip the operator character
    }
    
    return ft_append_to_string_array(files, filename);
}

t_tree *parse_cmd(t_token *tokens) {
    t_tree *node = NULL;
    t_token *current = tokens;
    
    node = (t_tree *)malloc(sizeof(t_tree));
    if (!node) 
        return NULL;
    
    // Initialize all fields to NULL
    ft_memset(node, 0, sizeof(t_tree));
    node->type = NODE_COMMAND;
    
    while (current) {
        if (current->type == TOKEN_REDIR_IN || current->type == TOKEN_HEREDOC)
            node->data.command.infiles = handle_redirection(&node->data.command.infiles, &current);
        else if (current->type == TOKEN_REDIR_OUT || current->type == TOKEN_APPEND)
            node->data.command.outfiles = handle_redirection(&node->data.command.outfiles, &current);
        else if (current->type == TOKEN_WORD)
            node->data.command.cmd = ft_append_to_string_array(&node->data.command.cmd, current->value);
        
        current = current->next;
    }
    
    return node;
}

t_tree *parse_bin_op(t_token *tokens) {
    t_tree *node = NULL;

    node = (t_tree *)malloc(sizeof(t_tree));
    if (!node) 
        return NULL;
    //
 // Initialize ALL fields (very important)
    ft_memset(node, 0, sizeof(t_tree));
    
    if (tokens->type == TOKEN_AND)
        node->type = NODE_AND;
    else if (tokens->type ==  TOKEN_OR)
        node->type = NODE_OR;
    else if (tokens->type == TOKEN_PIPE)
        node->type = NODE_PIPE;
    else 
      return NULL;

    node->data.operator = tokens->value;
    node->right = NULL;
    node->left = NULL;

    return node;
}

t_tree *parse_pipe(t_token *tokens) {
    t_tree *data_tree = NULL;
    t_token *left_tokens = tokens;
    
    // Search for PIPE operators
    while (tokens && tokens->type != TOKEN_PIPE)
        tokens = tokens->next;
    
    // If no operator found, move to next precedence level
    if (!tokens)
        return (parse_cmd(left_tokens));
    
    // Create node for the operator
    data_tree = parse_bin_op(tokens);
    
    // Break the token list
    if (tokens->prev) {
        tokens->prev->next = NULL;
        tokens->prev = NULL;
    }
    
    // Handle left side (check if first token was operator)
    if (left_tokens == tokens)
        data_tree->left = NULL;
    else 
        data_tree->left = parse_cmd(left_tokens);

    
    // Handle right side (check if operator was last token)
    if (tokens->next)
        data_tree->right =  parse_cmd(tokens->next);
    else
        data_tree->right =  NULL;
    
    return data_tree;
}
// Parse expressions with AND/OR
t_tree *parse_and_or(t_token *tokens) {
    t_tree *data_tree = NULL;
    t_token *left_tokens = tokens;
    
    // Search for AND/OR operators
    while (tokens && tokens->type != TOKEN_AND && tokens->type != TOKEN_OR)
        tokens = tokens->next;
    
    // If no operator found, move to next precedence level
    if (!tokens)
        return (parse_pipe(left_tokens));
    
    // Create node for the operator
    data_tree = parse_bin_op(tokens);
    
    // Break the token list
    if (tokens->prev) {
        tokens->prev->next = NULL;
        tokens->prev = NULL;
    }
    
    // Handle left side (check if first token was operator)
    if (left_tokens == tokens)
        data_tree->left = NULL;
    else 
        data_tree->left = parse_pipe(left_tokens);

    
    // Handle right side (check if operator was last token)
    if (tokens->next)
        data_tree->right = parse_and_or(tokens->next);
    else
        data_tree->right =  NULL;
    
    return data_tree;
}

// Main parser function
t_tree *parser(t_token *tokens) {
    // Start parsing at the lowest precedence level
    // return parse_and_or(tokens);
    return parse_and_or(tokens);
}
