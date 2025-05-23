#include "../../Inc/minishell.h"
#include <stdlib.h>
#include "../../libft/libft.h"

// ========== TOKEN LIST UTILITIES ==========

t_token *find_token_type(t_token *tokens, t_token_type types[], int type_count)
{
    int i;
    
    while (tokens)
    {
        i = 0;
        while (i < type_count)
        {
            if (tokens->type == types[i])
                return tokens;
            i++;
        }
        tokens = tokens->next;
    }
    return NULL;
}

t_token *split_token_list_at(t_token *split_point, t_token **left_tokens)
{
    t_token *right_tokens;
    t_token *head;
    
    if (!split_point)
    {
        *left_tokens = NULL;
        return NULL;
    }
    
    // Get right side (after split point)
    right_tokens = split_point->next;
    if (right_tokens)
        right_tokens->prev = NULL;
    
    // Disconnect split point
    split_point->next = NULL;
    
    // Get left side (before split point)
    if (split_point->prev)
    {
        split_point->prev->next = NULL;
        head = split_point;
        while (head->prev)
            head = head->prev;
        *left_tokens = head;
    }
    else
    {
        *left_tokens = NULL;
    }
    split_point->prev = NULL;
    
    return right_tokens;
}

t_token *clone_token_list(t_token *original)
{
    t_token *head = NULL;
    t_token *tail = NULL;
    t_token *new_token;
    t_token *current;
    
    current = original;
    while (current)
    {
        new_token = malloc(sizeof(t_token));
        if (!new_token)
        {
            free_token_list(head);
            return NULL;
        }
        new_token->value = ft_strdup(current->value);
        if (!new_token->value)
        {
            free(new_token);
            free_token_list(head);
            return NULL;
        }
        new_token->type = current->type;
        new_token->next = NULL;
        new_token->prev = tail;
        
        if (!head)
            head = new_token;
        else
            tail->next = new_token;
        tail = new_token;
        
        current = current->next;
    }
    return head;
}

// ========== TREE NODE MANAGEMENT ==========

t_tree *create_tree_node_with_tokens(t_node_type type, t_token *tokens)
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

t_tree *create_operator_node(t_token *op_token)
{
    t_node_type node_type;
    
    if (!op_token)
        return NULL;
    
    switch (op_token->type)
    {
        case TOKEN_PIPE:
            node_type = NODE_PIPE;
            break;
        case TOKEN_AND:
            node_type = NODE_AND;
            break;
        case TOKEN_OR:
            node_type = NODE_OR;
            break;
        default:
            node_type = NODE_COMMAND;
    }
    
    return create_tree_node_with_tokens(node_type, op_token);
}

t_tree *create_command_node(t_token *token_list)
{
    if (!token_list)
        return NULL;
    
    // Handle parentheses specially
    if (token_list->type == TOKEN_PAREN && !token_list->next)
    {
        t_tree *paren_node;
        t_token *inner_tokens;
        
        // Parse content inside parentheses
        inner_tokens = lexer(token_list->value);
        paren_node = create_tree_node_with_tokens(NODE_PAREN, token_list);
        if (paren_node && inner_tokens)
            paren_node->left = parse_expression(inner_tokens);
        
        return paren_node;
    }
    
    return create_tree_node_with_tokens(NODE_COMMAND, token_list);
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

// ========== PARSING FUNCTIONS ==========

t_tree *parse_command_sequence(t_token *tokens)
{
    if (!tokens)
        return NULL;
    
    return create_command_node(tokens);
}

t_tree *parse_pipeline(t_token *tokens)
{
    t_token_type pipe_types[] = {TOKEN_PIPE};
    t_token *pipe_op;
    t_token *left_tokens;
    t_token *right_tokens;
    t_tree *pipe_node;
    
    if (!tokens)
        return NULL;
    
    pipe_op = find_token_type(tokens, pipe_types, 1);
    if (!pipe_op)
        return parse_command_sequence(tokens);
    
    right_tokens = split_token_list_at(pipe_op, &left_tokens);
    
    pipe_node = create_operator_node(pipe_op);
    if (!pipe_node)
        return NULL;
    
    pipe_node->left = parse_command_sequence(left_tokens);
    pipe_node->right = parse_pipeline(right_tokens);
    
    return pipe_node;
}

t_tree *parse_logical_ops(t_token *tokens)
{
    t_token_type logical_types[] = {TOKEN_AND, TOKEN_OR};
    t_token *logical_op;
    t_token *left_tokens;
    t_token *right_tokens;
    t_tree *logical_node;
    
    if (!tokens)
        return NULL;
    
    logical_op = find_token_type(tokens, logical_types, 2);
    if (!logical_op)
        return parse_pipeline(tokens);
    
    right_tokens = split_token_list_at(logical_op, &left_tokens);
    
    logical_node = create_operator_node(logical_op);
    if (!logical_node)
        return NULL;
    
    logical_node->left = parse_logical_ops(left_tokens);
    logical_node->right = parse_logical_ops(right_tokens);
    
    return logical_node;
}

t_tree *parse_expression(t_token *tokens)
{
    return parse_logical_ops(tokens);
}

// ========== SYNTAX VALIDATION ==========

int validate_token_sequence(t_token *tokens)
{
    t_token *current;
    t_token *prev;
    
    if (!tokens)
        return 1;
    
    current = tokens;
    prev = NULL;
    
    while (current)
    {
        // Check for consecutive operators
        if (prev && 
            (prev->type == TOKEN_PIPE || prev->type == TOKEN_AND || prev->type == TOKEN_OR) &&
            (current->type == TOKEN_PIPE || current->type == TOKEN_AND || current->type == TOKEN_OR))
        {
            ft_printf("syntax error near unexpected token `%s'\n", current->value);
            return 0;
        }
        
        // Check for operators at start
        if (!prev && 
            (current->type == TOKEN_PIPE || current->type == TOKEN_AND || current->type == TOKEN_OR))
        {
            ft_printf("syntax error near unexpected token `%s'\n", current->value);
            return 0;
        }
        
        prev = current;
        current = current->next;
    }
    
    // Check for operators at end
    if (prev && 
        (prev->type == TOKEN_PIPE || prev->type == TOKEN_AND || prev->type == TOKEN_OR))
    {
        ft_printf("syntax error near unexpected token `newline'\n");
        return 0;
    }
    
    return 1;
}

int validate_parentheses_balance(t_token *tokens)
{
    int paren_count;
    t_token *current;
    
    paren_count = 0;
    current = tokens;
    
    while (current)
    {
        if (current->type == TOKEN_PAREN)
        {
            // Count opening and closing parens in the content
            char *content = current->value;
            while (*content)
            {
                if (*content == '(') paren_count++;
                else if (*content == ')') paren_count--;
                content++;
            }
        }
        current = current->next;
    }
    
    if (paren_count != 0)
    {
        ft_printf("syntax error: unmatched parentheses\n");
        return 0;
    }
    
    return 1;
}

int validate_redirections(t_token *tokens)
{
    t_token *current;
    
    current = tokens;
    while (current)
    {
        if (current->type == TOKEN_REDIR_IN || current->type == TOKEN_REDIR_OUT ||
            current->type == TOKEN_APPEND || current->type == TOKEN_HEREDOC)
        {
            if (!current->next || 
                (current->next->type != TOKEN_WORD && current->next->type != TOKEN_LITERAL_WORD))
            {
                ft_printf("syntax error near unexpected token `newline'\n");
                return 0;
            }
        }
        current = current->next;
    }
    
    return 1;
}

int validate_syntax(t_token *tokens)
{
    if (!validate_token_sequence(tokens))
        return 0;
    if (!validate_parentheses_balance(tokens))
        return 0;
    if (!validate_redirections(tokens))
        return 0;
    
    return 1;
}

// ========== PUBLIC INTERFACE ==========

t_tree *parser(t_token *tokens)
{
    t_token *token_copy;
    t_tree *syntax_tree;
    
    if (!tokens)
        return NULL;
    
    if (!validate_syntax(tokens))
        return NULL;
    
    // Clone tokens to preserve original list for caller
    token_copy = clone_token_list(tokens);
    if (!token_copy)
        return NULL;
    
    syntax_tree = parse_expression(token_copy);
    
    return syntax_tree;
}
