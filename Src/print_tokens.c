#include "../Inc/minishell.h"
#include "../libft/libft.h" 

// const char *get_token_type_string(t_token_type type) {
//     static const char *token_names[] = {
//         "REDIR_IN", "REDIR_OUT", "APPEND", "HEREDOC", 
//         "AND", "OR", "PIPE", "OPEN_PAREN", "CLOSE_PAREN",
//         "WORD", "EOF"
//     };
//     return token_names[type];
// }
//
// void print_token_list(t_token *head) {
//     t_token *current = head;
//     int i = 0;
//
//     while (current) {
//         printf("[%d] Type: %-15s Value: \"%s\"\n", 
//                i++, get_token_type_string(current->type), current->value);
//         current = current->next;
//     }
// }
//
