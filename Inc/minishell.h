#ifndef MINISHELL_H
#define MINISHELL_H


//struct
typedef struct s_tree {
    char *infiles;
    char* cmd;
    char* options;
    char *outfiles;
    void* right;
    void* left;
} t_tree;

typedef enum e_token_type {
    TOKEN_WORD,        // Palabras generales (comandos, argumentos)
    TOKEN_PIPE,        // '|'
    TOKEN_REDIR_IN,    // '<'
    TOKEN_REDIR_OUT,   // '>'
    TOKEN_APPEND,      // '>>'
    TOKEN_HEREDOC,     // '<<'
    TOKEN_AND,         // '&&'
    TOKEN_OR,          // '||'
    TOKEN_OPEN_PAREN,  // '('
    TOKEN_CLOSE_PAREN, // ')'
    TOKEN_EOF      // Fin de entrada
} t_token_type;

typedef struct s_token {
    char            *value; // Texto del token
    t_token_type     type;  // Tipo de token
    struct s_token  *next;  // Siguiente token en la lista
} t_token;

// Funciones
t_token* lexer(char *);
t_tree* parser(t_token*);
void executor(t_tree*, char **);

#endif
