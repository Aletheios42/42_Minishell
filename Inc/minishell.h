#ifndef MINISHELL_H
#define MINISHELL_H

#ifndef BUFF_SIZE
# define BUFF_SIZE 1024


// #include "lexer.h"
#include "lexer.h"
#include "parser.h"




typedef struct s_env // estructura para el env
{
    char            *value;
    struct s_env    *next;
}   t_env;

// Funciones
t_token* lexer(char *);
t_tree* parser(t_token*);
void executor(t_tree*, char **);


int ft_print_tree(t_tree *, int );
void print_token_list(t_token *head);
const char *get_token_type_string(t_token_type type);
// int ft_print_token_list(t_token *);
#endif
#endif
