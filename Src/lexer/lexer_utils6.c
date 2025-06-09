/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils6.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elorente <elorente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 00:31:01 by alepinto          #+#    #+#             */
/*   Updated: 2025/06/09 19:46:09 by elorente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "../Inc/minishell.h"
#include <readline/readline.h>
#include <readline/history.h>

int	process_token_char(char **input, t_tokenizer *tok)
{
	if (process_quote_char(&tok->ctx, **input))
	{
		if (!tok->ctx.in_quotes)
			tok->type = tok->ctx.quote_type;
		(*input)++;
		return (1);
	}
	if (!tok->ctx.in_quotes && is_ifs_char(**input))
		return (handle_ifs_split(input, tok));
	if (!tok->ctx.in_quotes && get_operator_at_position(*input))
		return (handle_operator(input, tok));
	if (!append_char_to_builder(tok->builder, **input))
	{
		free_builder(tok->builder);
		return (0);
	}
	(*input)++;
	return (1);
}
