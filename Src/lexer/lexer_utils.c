/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elorente <elorente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 22:43:01 by alepinto          #+#    #+#             */
/*   Updated: 2025/06/11 19:02:46 by elorente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "../Inc/minishell.h"
#include <readline/readline.h>
#include <readline/history.h>

char	*get_operator_at_position(char *str)
{
	if (ft_strncmp(str, "&&", 2) == 0)
		return ("&&");
	if (ft_strncmp(str, "||", 2) == 0)
		return ("||");
	if (ft_strncmp(str, "<<", 2) == 0)
		return ("<<");
	if (ft_strncmp(str, ">>", 2) == 0)
		return (">>");
	if (ft_strncmp(str, "|", 1) == 0)
		return ("|");
	if (ft_strncmp(str, ";", 1) == 0)
		return (";");
	if (ft_strncmp(str, "<", 1) == 0)
		return ("<");
	if (ft_strncmp(str, ">", 1) == 0)
		return (">");
	if (*str == '(')
		return ("(");
	if (*str == ')')
		return (")");
	return (NULL);
}

t_token_type	get_operator_token_type(char *op)
{
	if (ft_strcmp(op, "&&") == 0)
		return (TOKEN_AND);
	if (ft_strcmp(op, "||") == 0)
		return (TOKEN_OR);
	if (ft_strcmp(op, "|") == 0)
		return (TOKEN_PIPE);
	if (ft_strcmp(op, "<<") == 0)
		return (TOKEN_HEREDOC);
	if (ft_strcmp(op, ">>") == 0)
		return (TOKEN_APPEND);
	if (ft_strcmp(op, "<") == 0)
		return (TOKEN_REDIR_IN);
	if (ft_strcmp(op, ">") == 0)
		return (TOKEN_REDIR_OUT);
	if (ft_strcmp(op, "(") == 0)
		return (TOKEN_PAREN_OPEN);
	if (ft_strcmp(op, ")") == 0)
		return (TOKEN_PAREN_CLOSE);
	return (TOKEN_WORD);
}

// ========== QUOTE PROCESSING ==========

void	init_quote_context(t_quote_context *ctx)
{
	ctx->quote_char = 0;
	ctx->in_quotes = 0;
	ctx->quote_type = TOKEN_WORD;
}

int	process_quote_char(t_quote_context *ctx, char c)
{
	if (!ctx->in_quotes && is_quote_char(c))
	{
		ctx->in_quotes = 1;
		ctx->quote_char = c;
		if (c == '\'')
			ctx->quote_type = TOKEN_LITERAL_WORD;
		else
			ctx->quote_type = TOKEN_WORD;
		return (1);
	}
	else if (ctx->in_quotes && c == ctx->quote_char)
	{
		ctx->in_quotes = 0;
		ctx->quote_char = 0;
		return (1);
	}
	return (0);
}

// ========== TOKEN MANAGEMENT ==========
t_token	*create_token(char *value, t_token_type type)
{
	t_token	*token;

	if (!value || *value == '\0')
		return (NULL);
	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->value = value;
	token->type = type;
	token->next = NULL;
	token->prev = NULL;
	return (token);
}
