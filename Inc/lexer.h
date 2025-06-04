/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elorente <elorente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 23:26:14 by elorente          #+#    #+#             */
/*   Updated: 2025/06/04 23:26:14 by elorente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

# include "structs.h"

// Lexer function prototypes
//lexer.c

int					is_ifs_char(char c);
int					is_logical_operator(char *str);
int					is_redirection_operator(char *str);
int					is_parenthesis_operator(char *str);
int					is_quote_char(char c);

//lexer_utils.c

char				*get_operator_at_position(char *str);
t_token_type		get_operator_token_type(char *op);
void				init_quote_context(t_quote_context *ctx);
int					process_quote_char(t_quote_context *ctx, char c);
t_token				*create_token(char *value, t_token_type type);

//lexer_utils2.c

void				add_token_to_list(t_token **head,
						t_token **tail, t_token *new_token);
void				free_token_list(t_token *head);
t_string_builder	*create_string_builder(void);
int					append_char_to_builder(t_string_builder *sb, char c);
char				*finalize_string_builder(t_string_builder *sb);

//lexer_utils3.c

char				*skip_whitespace(char *str);
int					check_unclosed_quotes(char *input);
int					check_unclosed_parens(char *input);
int					check_syntax_errors(char *input);
void				print_multiline_error(char *input);

//lexer_utils4.c

char				*handle_multiline_input(char *input);
int					add_current_token(t_string_builder *sb, t_token **head,
						t_token **tail, t_token_type token_type);
int					add_operator_token(char *op, t_token **head,
						t_token **tail);
t_token				*lexer(char *input);

//lexer_utils5.c

int					init_tokenizer(t_string_builder **builder,
						t_quote_context *ctx, t_token_type *type);
int					handle_ifs_split(char **input, t_tokenizer *tok);
int					handle_operator(char **input, t_tokenizer *tok);
void				free_builder(t_string_builder *builder);
t_token				*tokenize_input(char *input);

//lexer_utils6.c

int					process_token_char(char **input, t_tokenizer *tok);

#endif
