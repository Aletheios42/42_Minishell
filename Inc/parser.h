#ifndef PARSER_H
#define PARSER_H

// Estados del autómata para el análisis léxico
typedef enum {
    STATE_INITIAL,      // Estado inicial
    STATE_WORD,         // Procesando una palabra/comando
    STATE_IN_DQUOTE,    // Dentro de comillas doboes
    STATE_IN_SQUOTE,    // Dentro de comillas simples
    STATE_AFTER_ESCAPE, // Después de escape
    STATE_ERROR,        // Error de sintaxis
    STATE_ACCEPT        // Estado de aceptación
} e_dfa_state;

// Tipos de tokens
typedef enum e_token_type {
    TOKEN_REDIR_IN,    // '<'
    TOKEN_REDIR_OUT,   // '>'
    TOKEN_APPEND,      // '>>'
    TOKEN_HEREDOC,     // '<<'
    TOKEN_AND,         // '&&'
    TOKEN_OR,          // '||'
    TOKEN_PIPE,        // '|'
    TOKEN_OPEN_PAREN,  // '('
    TOKEN_CLOSE_PAREN, // ')'
    TOKEN_WORD,        // Palabras generales (comandos, argumentos, archivos ..)
    TOKEN_EOF          // Fin de entrada
} t_token_type;

// Estructura de token para almacenar información léxica
typedef struct s_token {
    char            *value; // Texto del token
    t_token_type     type;  // Tipo de token
    struct s_token  *next;  // Siguiente token en la lista
    struct s_token  *prev;  // Anterior token en la lista
} t_token;


// Node types for the abstract syntax tree
typedef enum {
    NODE_COMMAND,  // For regular commands
    NODE_PIPE,     // For pipe operator |
    NODE_AND,      // For logical AND &&
    NODE_OR,       // For logical OR ||
    NODE_PAREN     // For parenthesized groups
} t_node_type;

// Abstract syntax tree node
typedef struct s_tree {
    t_node_type type;         // Type of node
    t_token *tokens;          // Token list for this node (commands, redirections)
    struct s_tree* left;      // Left child
    struct s_tree* right;     // Right child
} t_tree;



// Main parser functions
t_tree* parser(char *);
t_tree* parse_and_or(t_token *tokens);
t_tree* parse_pipe(t_token *tokens);
t_tree* create_tree_node(t_token *token);

// lexer.c Prototipos de funciones
t_token* lexer(char *input);
t_token* create_token(char *value, t_token_type type);
void add_token(t_token **head, t_token **current, t_token *new_token);
t_token_type determine_token_type(char *value);

// Parser utilities
t_token* split_token_list(t_token *token, t_token **left_list);
t_token* search_first_occurrence_token_type(t_token *tokens, t_token_type types[], int type_count);
void     free_syntax_tree(t_tree *tree);

// Tree printing
int ft_print_syntax_tree(t_tree *tree, int depth);
int ft_print_tree_indent(int depth);
int ft_print_string_array(char **array);

#endif
