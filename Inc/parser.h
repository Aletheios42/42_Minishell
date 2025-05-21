#ifndef PARSER_H
#define PARSER_H

// Token types enum
typedef enum e_token_type {
    TOKEN_REDIR_IN,    // '<'
    TOKEN_REDIR_OUT,   // '>'
    TOKEN_APPEND,      // '>>'
    TOKEN_HEREDOC,     // '<<'
    TOKEN_AND,         // '&&'
    TOKEN_OR,          // '||'
    TOKEN_PIPE,        // '|'
    TOKEN_PAREN,       // '(' content ')'
    TOKEN_WORD,        // Regular words and double-quoted content
    TOKEN_LITERAL_WORD, // Single-quoted content (literal, no expansion)
    TOKEN_EOF          // End of input
} t_token_type;

// Token structure
typedef struct s_token {
    char            *value; // Token text
    t_token_type     type;  // Token type
    struct s_token  *next;  // Next token in list
    struct s_token  *prev;  // Previous token in list
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
    t_token *tokens;          // Token list for this node
    struct s_tree* left;      // Left child
    struct s_tree* right;     // Right child
} t_tree;

// Lexer function prototypes
t_token* lexer(char *input);
t_token* create_token(char *value, t_token_type type);
void add_token(t_token **head, t_token **current, char *value, t_token_type type);
char *identify_delimiter(char *str, char **delims);
char *skip_spaces(char *str);
t_token_type get_operator_type(char *op);
t_token_type get_redirection_type(char *value);
int is_redirection(char *value);
char **get_delimiters(void);

// Parser function prototypes
t_tree* parser(char *input);
t_tree* parse_and_or(t_token *tokens);
t_tree* parse_pipe(t_token *tokens);
t_tree* create_tree_node(t_token *token);

// Parser utilities
t_token* split_token_list(t_token *token, t_token **left_list);
t_token* search_first_occurrence_token_type(t_token *tokens, t_token_type types[], int type_count);
void free_syntax_tree(t_tree *tree);

// Printing functions
int ft_print_syntax_tree(t_tree *tree, int depth);
int ft_print_tree_indent(int depth);
int ft_print_string_array(char **array);
int ft_print_token_list(t_token *head);
const char *get_token_type_string(t_token_type type);
void print_token_list(t_token *head);

#endif
