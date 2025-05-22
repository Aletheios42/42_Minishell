#include "../../Inc/minishell.h"

// char *skip_spaces(char *str)
// {
//     while (str && *str && (*str == ' ' || *str == '\t'))
//         str++;
//     return str;
// }
//
//
// char **get_delimiters(void)
// {
//     static char *delims[] = {" ", "\t", "<<", ">>", "<", ">", "(", ")", 
//                           "\"", "'", "&&", "|", "||", ";", "&", NULL};
//     return delims;
// }
//
//
// void add_token_back(t_token **tokens, t_token *new_token)
// {
//     t_token *current;
//
//     if (!new_token)
//         return;
//
//     if (!*tokens)
//     {
//         *tokens = new_token;
//         return;
//     }
//
//     current = *tokens;
//     while (current->next)
//         current = current->next;
//
//     current->next = new_token;
//     new_token->prev = current;
// }
//
// t_token *create_token(char *value, t_token_type type)
// {
//     t_token *node;
//
//     if (!value)
//         return NULL;
//     node = (t_token *)malloc(sizeof(t_token));
//     if (!node)
//         return NULL;
//     node->value = value;
//     node->type = type;
//     node->next = NULL;
//     node->prev = NULL;
//     return node;
// }
