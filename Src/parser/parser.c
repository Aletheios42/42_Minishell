#include "../../Inc/minishell.h"
#include <stdlib.h>
#include "../../libft/libft.h"

// ========== TOKEN LIST UTILITIES ==========

t_token *find_token_type(t_token *tokens, t_token_type type)
{
    while (tokens)
    {
        if (tokens->type == type)
            return tokens;
        tokens = tokens->next;
    }
    return NULL;
}

t_token *find_logical_operator(t_token *tokens)
{
    int paren_level = 0;
    
    while (tokens)
    {
        if (tokens->type == TOKEN_PAREN_OPEN)
            paren_level++;
        else if (tokens->type == TOKEN_PAREN_CLOSE)
            paren_level--;
        else if (paren_level == 0 && 
                (tokens->type == TOKEN_AND || tokens->type == TOKEN_OR))
            return tokens;
        tokens = tokens->next;
    }
    return NULL;
}

t_token *find_pipe_operator(t_token *tokens)
{
    int paren_level = 0;
    
    while (tokens)
    {
        if (tokens->type == TOKEN_PAREN_OPEN)
            paren_level++;
        else if (tokens->type == TOKEN_PAREN_CLOSE)
            paren_level--;
        else if (paren_level == 0 && tokens->type == TOKEN_PIPE)
            return tokens;
        tokens = tokens->next;
    }
    return NULL;
}

t_token *cut_token_list_at(t_token **head, t_token *cut_point)
{
    t_token *right_part;
    
    if (!cut_point)
        return NULL;
    
    right_part = cut_point->next;
    
    // Disconnect the lists
    cut_point->next = NULL;
    if (right_part)
        right_part->prev = NULL;
    
    // Update head if necessary
    if (cut_point->prev)
        cut_point->prev->next = NULL;
    else
        *head = NULL;
    
    cut_point->prev = NULL;
    return right_part;
}

// ========== TREE NODE MANAGEMENT ==========

t_tree *create_tree_node(t_node_type type, t_token *tokens)
{
    t_tree *node;
    
    node = malloc(sizeof(t_tree));
    if (!node)
        return NULL;
    
    node->type = type;
    node->tokens = tokens;
    node->left = NULL;
    node->right = NULL;
    return node;
}

void free_syntax_tree(t_tree *tree)
{
    if (!tree)
        return;
    
    free_syntax_tree(tree->left);
    free_syntax_tree(tree->right);
    
    if (tree->tokens)
        free_token_list(tree->tokens);
    
    free(tree);
}

// ========== PARENTHESES PARSING ==========

t_token *find_matching_close_paren(t_token *open_paren)
{
    t_token *current;
    int paren_level = 1;
    
    if (!open_paren)
        return NULL;
    
    current = open_paren->next;
    while (current && paren_level > 0)
    {
        if (current->type == TOKEN_PAREN_OPEN)
            paren_level++;
        else if (current->type == TOKEN_PAREN_CLOSE)
        {
            paren_level--;
            if (paren_level == 0)
                return current;
        }
        current = current->next;
    }
    return NULL;
}

t_token *extract_and_cut_inner_tokens(t_token *open_paren, t_token *close_paren)
{
    t_token *inner_start;
    t_token *inner_end;
    
    if (!open_paren || !close_paren)
        return NULL;
    
    inner_start = open_paren->next;
    if (!inner_start || inner_start == close_paren)
        return NULL;
    
    inner_end = close_paren->prev;
    if (!inner_end)
        return NULL;
    
    // Cut inner tokens from the list
    open_paren->next = close_paren;
    close_paren->prev = open_paren;
    
    // Make inner tokens a separate list
    inner_start->prev = NULL;
    inner_end->next = NULL;
    
    return inner_start;
}

t_token *extract_and_cut_external_redirections(t_token *close_paren)
{
    t_token *external_start;
    
    if (!close_paren || !close_paren->next)
        return NULL;
    
    external_start = close_paren->next;
    
    // Cut external tokens
    close_paren->next = NULL;
    external_start->prev = NULL;
    
    return external_start;
}

t_tree *parse_parentheses_group(t_token **tokens)
{
    t_token *open_paren;
    t_token *close_paren;
    t_token *inner_tokens;
    t_token *external_tokens;
    t_tree *paren_node;
    
    open_paren = find_token_type(*tokens, TOKEN_PAREN_OPEN);
    if (!open_paren)
        return NULL;
    
    close_paren = find_matching_close_paren(open_paren);
    if (!close_paren)
        return NULL;
    
    // Extract tokens (no cloning, just cutting)
    inner_tokens = extract_and_cut_inner_tokens(open_paren, close_paren);
    external_tokens = extract_and_cut_external_redirections(close_paren);
    
    // Remove parentheses tokens from original list
    if (open_paren->prev)
        open_paren->prev->next = close_paren->next;
    else
        *tokens = close_paren->next;
    
    if (close_paren->next)
        close_paren->next->prev = open_paren->prev;
    
    // Free parentheses tokens
    free(open_paren->value);
    free(open_paren);
    free(close_paren->value);
    free(close_paren);
    
    paren_node = create_tree_node(NODE_PAREN, external_tokens);
    if (!paren_node)
    {
        free_token_list(inner_tokens);
        free_token_list(external_tokens);
        return NULL;
    }
    
    paren_node->right = parse_expression(inner_tokens);
    return paren_node;
}

// ========== PARSING FUNCTIONS ==========

t_tree *parse_command(t_token *tokens)
{
    if (!tokens)
        return NULL;
    
    if (find_token_type(tokens, TOKEN_PAREN_OPEN))
        return parse_parentheses_group(&tokens);
    
    return create_tree_node(NODE_COMMAND, tokens);
}

t_tree *parse_pipeline(t_token *tokens)
{
    t_token *pipe_op;
    t_token *right_tokens;
    t_tree *pipe_node;
    
    pipe_op = find_pipe_operator(tokens);
    if (!pipe_op)
        return parse_command(tokens);
    
    right_tokens = cut_token_list_at(&tokens, pipe_op);
    
    pipe_node = create_tree_node(NODE_PIPE, pipe_op);
    if (!pipe_node)
        return NULL;
    
    pipe_node->left = parse_command(tokens);
    pipe_node->right = parse_pipeline(right_tokens);
    return pipe_node;
}

t_tree *parse_logical_ops(t_token *tokens)
{
    t_token *logical_op;
    t_token *right_tokens;
    t_tree *logical_node;
    t_node_type node_type;
    
    logical_op = find_logical_operator(tokens);
    if (!logical_op)
        return parse_pipeline(tokens);
    
    right_tokens = cut_token_list_at(&tokens, logical_op);
    node_type = (logical_op->type == TOKEN_AND) ? NODE_AND : NODE_OR;
    
    logical_node = create_tree_node(node_type, logical_op);
    if (!logical_node)
        return NULL;
    
    logical_node->left = parse_logical_ops(tokens);
    logical_node->right = parse_logical_ops(right_tokens);
    return logical_node;
}

t_tree *parse_expression(t_token *tokens)
{
    return parse_logical_ops(tokens);
}

// ========== SYNTAX VALIDATION ==========

int validate_parentheses(t_token *tokens)
{
    int paren_count = 0;
    
    while (tokens)
    {
        if (tokens->type == TOKEN_PAREN_OPEN)
            paren_count++;
        else if (tokens->type == TOKEN_PAREN_CLOSE)
            paren_count--;
        
        if (paren_count < 0)
        {
            ft_printf("syntax error: unexpected ')'\n");
            return 0;
        }
        tokens = tokens->next;
    }
    
    if (paren_count != 0)
    {
        ft_printf("syntax error: unmatched '('\n");
        return 0;
    }
    return 1;
}

int validate_operators(t_token *tokens)
{
    t_token *prev = NULL;
    
    while (tokens)
    {
        if (prev && 
            (prev->type == TOKEN_PIPE || prev->type == TOKEN_AND || 
             prev->type == TOKEN_OR) &&
            (tokens->type == TOKEN_PIPE || tokens->type == TOKEN_AND || 
             tokens->type == TOKEN_OR))
        {
            ft_printf("syntax error near unexpected token `%s'\n", 
                     tokens->value);
            return 0;
        }
        prev = tokens;
        tokens = tokens->next;
    }
    return 1;
}

int validate_redirections(t_token *tokens)
{
    while (tokens)
    {
        if (tokens->type == TOKEN_REDIR_IN || tokens->type == TOKEN_REDIR_OUT ||
            tokens->type == TOKEN_APPEND || tokens->type == TOKEN_HEREDOC)
        {
            if (!tokens->next || 
                (tokens->next->type != TOKEN_WORD && 
                 tokens->next->type != TOKEN_LITERAL_WORD))
            {
                ft_printf("syntax error near unexpected token `newline'\n");
                return 0;
            }
        }
        tokens = tokens->next;
    }
    return 1;
}

int validate_syntax(t_token *tokens)
{
    if (!validate_parentheses(tokens))
        return 0;
    if (!validate_operators(tokens))
        return 0;
    if (!validate_redirections(tokens))
        return 0;
    return 1;
}

// ========== PUBLIC INTERFACE ==========

t_tree *parser(t_token *tokens)
{
    if (!tokens)
        return NULL;
    
    if (!validate_syntax(tokens))
        return NULL;
    
    return parse_expression(tokens);
}
