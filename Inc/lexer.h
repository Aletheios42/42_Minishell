#ifndef LEXER_H
#define LEXER_H

#define MAX_STATES 8     // Número máximo de estados
#define MAX_SYMBOLS 11   // Número máximo de símbolos (tokens)

typedef struct {
    int (*transition_table)[MAX_SYMBOLS]; // Puntero a matriz
    int *final_states;                    // Puntero a array
    int initial_state;
    int current_state;
    void *context;
} DFA;

typedef enum {
    STATE_INITIAL,      // Estado inicial
    STATE_IN_PAREN,     // Dentro de paréntesis
    STATE_IN_DQUOTE,    // Dentro de comillas dobles
    STATE_IN_SQUOTE,    // Dentro de comillas simples
    STATE_AFTER_OP,     // Después de cualquier operador (pipe, redir, &&, ||)
    STATE_WORD,         // Procesando una palabra/comando
    STATE_ERROR,        // Error de sintaxis
    STATE_ACCEPT        // Estado de aceptación
} e_dfa_state;

// Alfabeto
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

// Estruct para traducir el input
typedef struct s_token {
    char            *value; // Texto del token
    t_token_type     type;  // Tipo de token
    struct s_token  *next;  // Siguiente token en la lista
    struct s_token  *prev;  // Anterior token en la lista
} t_token;

#endif
