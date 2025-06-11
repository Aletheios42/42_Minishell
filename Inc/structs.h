/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elorente <elorente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 20:23:06 by alepinto          #+#    #+#             */
/*   Updated: 2025/06/11 21:02:35 by elorente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTS_H
# define STRUCTS_H

// Token types enum
typedef enum e_token_type
{
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
	TOKEN_EOF,			// End of input
}	t_token_type;

// Token structure
typedef struct s_token
{
	char			*value;	// Token text
	t_token_type	type;	// Token type
	struct s_token	*next;	// Next token in list
	struct s_token	*prev;	// Previous token in list
}	t_token;

// Node types for the abstract syntax tree
typedef enum e_node_type
{
	NODE_COMMAND,	// For regular commands
	NODE_PIPE,		// For pipe operator |
	NODE_AND,		// For logical AND &&
	NODE_OR,		// For logical OR ||
	NODE_PAREN,		// For parenthesized groups
}	t_node_type;

typedef enum e_command_type
{
	CMD_ASSIGNMENT,
	CMD_BUILTIN,
	CMD_EXTERNAL,
	CMD_INVALID
}	t_command_type;

// Abstract syntax tree node
typedef struct s_tree
{
	t_node_type		type;		// Type of node
	t_token			*tokens;		// Token list for this node
	struct s_tree	*left;	// Left child
	struct s_tree	*right;	// Right child
}	t_tree;

typedef struct s_env // estructura para el env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef struct s_expand_ctx
{
	const char	*line;
	int			*index;
	t_env		*env;
	int			exit_status;
}	t_expand_ctx;

typedef struct s_local_vars
{
	char					*key;
	char					*value;
	struct s_local_vars		*next;
}	t_local_vars;

typedef struct s_quote_context
{
	char					quote_char;
	int						in_quotes;
	t_token_type			quote_type;
}	t_quote_context;

// ========== STRING BUILDER ==========

typedef struct s_string_builder
{
	char					*str;
	size_t					len;
	size_t					capacity;
}	t_string_builder;

typedef struct s_tokenizer
{
	t_token				*head;
	t_token				*tail;
	t_string_builder	*builder;
	t_quote_context		ctx;
	t_token_type		type;
}	t_tokenizer;

typedef struct s_redir_ctx
{
	int	saved_stdin;
	int	saved_stdout;
}	t_redir_ctx;

typedef struct s_exec_context
{
	t_command_type	cmd_type;
	t_token			*expanded_tokens;
	char			**args;
	int				result;
	int				saved_stdin;
	int				saved_stdout;
	t_redir_ctx		redir_ctx;
}	t_exec_context;

typedef struct s_pipeline_ctx
{
	int		pipefd[2];
	pid_t	left_pid;
	pid_t	right_pid;
	int		status;
	int		saved_stdin;
	int		saved_stdout;
}	t_pipeline_ctx;

#endif
