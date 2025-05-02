#ifndef MINISHELL_H
#define MINISHELL_H


#include "lexer.h"
//struct
typedef struct s_tree {
    char *infiles;
    char* cmd;
    char* options;
    char *outfiles;
    void* right;
    void* left;
} t_tree;


typedef struct s_env // estructura para el env
{
    char            *value;
    struct s_env    *next;
}   t_env;

// Funciones
t_token* lexer(char *);
t_tree* parser(t_token*);
void executor(t_tree*, char **);


int ft_print_tree_indent(int depth);
int ft_print_tree(t_tree *tree, int depth);
int ft_print_token(t_token *token);
int ft_print_token_list(t_token *tokens);
#endif
