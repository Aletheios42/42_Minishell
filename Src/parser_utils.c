#include "../Inc/minishell.h"
#include <stdlib.h>
#include "../libft/libft.h"

// Split a token list at the given token
t_token *split_token_list(t_token *token, t_token **left_list)
{
    t_token *right_list = token->next;
    t_token *head = NULL;

    if (right_list)
        right_list->prev = NULL;
    token->next = NULL;
    
    if (token->prev) {
        head = token;
        while (head->prev)
            head = head->prev;
        
        *left_list = head;
        token->prev->next = NULL;
    } else {
        *left_list = NULL;
    }
    token->prev = NULL;
    
    return right_list;
}

// Search for first token of specified types
t_token *search_first_occurrence_token_type(t_token *tokens, t_token_type types[], int type_count)
{
    int i;
    while (tokens) {
        i = 0;
        while (i < type_count) {
            if (tokens->type == types[i])
                return tokens;
            i++;
        }
        tokens = tokens->next;
    }
    return NULL;
}


// Free a syntax tree
void free_syntax_tree(t_tree *tree)
{
    if (!tree)
        return;
    
    free_syntax_tree(tree->left);
    free_syntax_tree(tree->right);
    
    // Note: tokens are freed elsewhere
    free(tree);
}
