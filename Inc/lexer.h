#ifndef LEXER_H
#define LEXER_H

#include "structs.h"

// Lexer function prototypes
t_token* lexer(char *input);
t_token* create_token(char *value, t_token_type type);
void add_token(t_token **head, t_token **current, char *value, t_token_type type);
void add_token_back(t_token **tokens, t_token *new_token);
char *identify_delimiter(char *str, char **delims);
char *skip_spaces(char *str);
t_token_type get_operator_type(char *op);
t_token_type get_redirection_type(char *value);
int is_redirection(char *value);
char **get_delimiters(void);
void free_token_list(t_token *head);

#endif
