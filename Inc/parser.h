#ifndef PARSER_H
#define PARSER_H


typedef enum {
    NODE_COMMAND,  // For regular commands
    NODE_PIPE,     // For pipe operator |
    NODE_AND,      // For logical AND &&
    NODE_OR,       // For logical OR ||
    NODE_PAREN     // For parenthesized groups
} t_node_type;

//struct
typedef struct s_tree {
    t_node_type type;
    
    union {
        struct {
            char *infiles;
            char **cmd; 
            char *outfiles;
        } command;
        
        char *operator;
    } data;
    
    struct s_tree* right;
    struct s_tree* left;
} t_tree;


#endif
