/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils5.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elorente <elorente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 23:35:56 by elorente          #+#    #+#             */
/*   Updated: 2025/06/09 20:02:10 by elorente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "../Inc/minishell.h"
#include <readline/readline.h>
#include <readline/history.h>

int	init_tokenizer(t_string_builder **builder, t_quote_context *ctx,
	t_token_type *type)
{
	*builder = create_string_builder();
	if (!*builder)
		return (0);
	init_quote_context(ctx);
	*type = TOKEN_WORD;
	return (1);
}

int	handle_ifs_split(char **input, t_tokenizer *tok)
{
	tok->type = TOKEN_WORD;
	if (!add_current_token(tok->builder, &tok->head, &tok->tail, tok->type))
		return (0);
	free_builder(tok->builder);
	tok->builder = create_string_builder();
	if (!tok->builder)
		return (0);
	tok->type = TOKEN_WORD;
	*input = skip_whitespace(*input);
	return (1);
}

int	handle_operator(char **input, t_tokenizer *tok)
{
	char	*operator;

	operator = get_operator_at_position(*input);
	if (!operator)
		return (0);
	if (!add_current_token(tok->builder, &tok->head, &tok->tail, tok->type))
		return (0);
	if (!add_operator_token(operator, &tok->head, &tok->tail))
		return (0);
	free_builder(tok->builder);
	tok->builder = create_string_builder();
	if (!tok->builder)
		return (0);
	tok->type = TOKEN_WORD;
	*input += ft_strlen(operator);
	*input = skip_whitespace(*input);
	return (1);
}

void	free_builder(t_string_builder *builder)
{
	if (!builder)
		return ;
	if (builder->str)
		free(builder->str);
	free(builder);
}

t_token	*tokenize_input(char *input)
{
	t_tokenizer	tok;

	tok.head = NULL;
	tok.tail = NULL;
	if (!init_tokenizer(&tok.builder, &tok.ctx, &tok.type))
		return (NULL);
	input = skip_whitespace(input);
	while (*input)
	{
		if (!process_token_char(&input, &tok))
			return (free_token_list(tok.head), NULL);
	}
	if (!add_current_token(tok.builder, &tok.head, &tok.tail, tok.type))
		return (free_token_list(tok.head), NULL);
	free_builder(tok.builder);
	return (tok.head);
}
