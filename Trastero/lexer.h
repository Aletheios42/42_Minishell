#ifndef LEXER_H
#define LEXER_H

#define MAX_STATES 12
#define MAX_SYMBOLS 7

// Estructura para almacenar caracteres mientras construimos un token
typedef struct {
    char *buffer;       // Buffer para almacenar caracteres
    int position;       // Posición actual en el buffer
    int capacity;       // Capacidad máxima del buffer
} t_buffer;

// Estados del autómata para el análisis léxico
typedef enum {
    STATE_INITIAL,      // Estado inicial
    STATE_WORD,         // Procesando una palabra/comando
    STATE_AFTER_LT,     // Después de <
    STATE_AFTER_GT,     // Después de >
    STATE_AFTER_PIPE,   // Después de |
    STATE_AFTER_AMP,    // Después de &
    STATE_IN_DQUOTE,    // Dentro de comillas dobles
    STATE_IN_SQUOTE,    // Dentro de comillas simples
    STATE_AFTER_ESCAPE, // Después de escape
    STATE_ERROR,        // Error de sintaxis
    STATE_ACCEPT        // Estado de aceptación
} e_dfa_state;

// Tipos de tokens
typedef enum e_token_type {
    TOKEN_WORD,        // Palabras generales (comandos, argumentos)
    TOKEN_REDIR_IN,    // '<'
    TOKEN_REDIR_OUT,   // '>'
    TOKEN_APPEND,      // '>>'
    TOKEN_HEREDOC,     // '<<'
    TOKEN_AND,         // '&&'
    TOKEN_OR,          // '||'
    TOKEN_PIPE,        // '|'
    TOKEN_OPEN_PAREN,  // '('
    TOKEN_CLOSE_PAREN, // ')'
    TOKEN_EOF          // Fin de entrada
} t_token_type;

// Estructura de token para almacenar información léxica
typedef struct s_token {
    char            *value; // Texto del token
    t_token_type     type;  // Tipo de token
    struct s_token  *next;  // Siguiente token en la lista
    struct s_token  *prev;  // Anterior token en la lista
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
t_buffer create_buffer(int capacity);
void add_to_buffer(t_buffer *buffer, char c);
void reset_buffer(t_buffer *buffer);
t_token* create_token(char *value, t_token_type type);
void add_token(t_token **head, t_token **current, t_token *new_token);
t_token_type determine_token_type(char *value);

#endif
