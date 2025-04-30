#include "../Inc/minishell.h"


t_tree *parser(t_token* tokens) {
    t_tree* data_tree = 0x0;

    data_tree->cmd =  tokens->value;

    return data_tree; 
}
