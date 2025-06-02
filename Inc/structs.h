/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alepinto <alepinto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 20:23:06 by alepinto          #+#    #+#             */
/*   Updated: 2025/06/02 20:23:06 by alepinto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCS_H
#define STRUCS_H

// Token types enum
typedef enum e_token_type {
	TOKEN_REDIR_IN,		// '<'
	TOKEN_REDIR_OUT,	// '>'
	TOKEN_APPEND,		// '>>'
	TOKEN_HEREDOC,		// '<<'
	TOKEN_AND,			// '&&'
	TOKEN_OR,			// '||'
	TOKEN_PIPE,			// '|'
	TOKEN_PAREN_OPEN,	// '('
	TOKEN_PAREN_CLOSE,	// ')'
	TOKEN_WORD,			// Regular words and double-quoted content
	TOKEN_LITERAL_WORD,	// Single-quoted content (literal, no expansion)
	TOKEN_EOF			// End of input
}	t_token_type;

// Token structure
typedef struct s_token {
	char			*value; // Token text
	t_token_type	type;  // Token type
	struct s_token	*next;  // Next token in list
	struct s_token	*prev;  // Previous token in list
}	t_token;

// Node types for the abstract syntax tree
typedef enum {
	NODE_COMMAND,  // For regular commands
	NODE_PIPE,     // For pipe operator |
	NODE_AND,      // For logical AND &&
	NODE_OR,       // For logical OR ||
	NODE_PAREN     // For parenthesized groups
}	t_node_type;

typedef enum	e_command_type
{
	CMD_ASSIGNMENT,
	CMD_BUILTIN,
	CMD_EXTERNAL,
	CMD_INVALID
}	t_command_type;

// Abstract syntax tree node
typedef struct s_tree {
	t_node_type type;         // Type of node
	t_token *tokens;          // Token list for this node
	struct s_tree* left;      // Left child
	struct s_tree* right;     // Right child
}	t_tree;

typedef struct s_env // estructura para el env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef struct s_local_vars
{
	char					*key;
	char					*value;
	struct s_local_vars		*next;
}		t_local_vars;

#endif
