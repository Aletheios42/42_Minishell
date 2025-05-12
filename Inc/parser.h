#ifndef PARSER_H
#define PARSER_H

// Node types for the AST
typedef enum {
    NODE_COMMAND,  // Regular command
    NODE_PIPE,     // Pipe operator |
    NODE_AND,      // Logical AND &&
    NODE_OR,       // Logical OR ||
    NODE_PAREN     // Parenthesized group
} t_node_type;

typedef enum e_redir_type {
   REDIR_IN,     // <
   REDIR_OUT,    // >  
   APPEND,       // >>
   HEREDOC       // <<
} t_redir_type;

// Redirection structure
typedef struct s_redir {
    t_redir_type type;       // REDIR_IN, REDIR_OUT, APPEND, HEREDOC 
    char *filename;          // Filename or heredoc delimiter
    struct s_redir *next;    // Next redirection
} t_redir;

// Command structure
typedef struct s_command {
    char **args;             // Command name and arguments
    t_redir *redirs;         // Linked list of redirections
} t_command;

// AST node structure
typedef struct s_tree {
    t_node_type type;   // Type of node
    
    union {
        char *operator;     // For operators (|, &&, ||)
        t_command command;  // For command nodes
    } data;
    
    struct s_tree *left;   // Left child
    struct s_tree *right;  // Right child
} t_tree;

#endif
