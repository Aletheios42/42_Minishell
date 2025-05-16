#ifndef LEXER_H
#define LEXER_H

#define MAX_STATES 8
#define MAX_SYMBOLS 11

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
} t_token;

// Estructura DFA para el autómata
typedef struct {
    int (*transition_table)[MAX_SYMBOLS]; // Tabla de transiciones
    int *final_states;                    // Estados finales
    int initial_state;                    // Estado inicial
    int current_state;                    // Estado actual
    void *context;                        // Contexto adicional
} DFA;

// Prototipos de funciones
t_token* lexer(char *input);
t_token* create_token(char *value, t_token_type type);
void add_token(t_token **head, t_token **current, t_token *new_token);
t_token_type determine_token_type(char *value);

#endif
