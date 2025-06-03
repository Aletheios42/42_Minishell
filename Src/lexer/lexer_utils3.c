/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alepinto <alepinto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 23:09:07 by alepinto          #+#    #+#             */
/*   Updated: 2025/06/03 23:09:07 by alepinto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "../Inc/minishell.h"
#include <readline/readline.h>
#include <readline/history.h>

// ========== INPUT VALIDATION ==========

char	*skip_whitespace(char *str)
{
	while (str && *str && is_ifs_char(*str))
		str++;
	return (str);
}

int	check_unclosed_quotes(char *input)
{
	t_quote_context	ctx;

	init_quote_context(&ctx);
	while (*input)
	{
		process_quote_char(&ctx, *input);
		input++;
	}
	return (ctx.in_quotes);
}

int	check_unclosed_parens(char *input)
{
	int				level;
	t_quote_context	ctx;

	level = 0;
	init_quote_context(&ctx);
	while (*input)
	{
		process_quote_char(&ctx, *input);
		if (!ctx.in_quotes)
		{
			if (*input == '(')
				level++;
			else if (*input == ')')
				level--;
		}
		input++;
	}
	return (level);
}

int	check_syntax_errors(char *input)
{
	char	*pos;

	pos = input;
	while (*pos)
	{
		if (ft_strncmp(pos, "|||", 3) == 0)
		{
			ft_printf("syntax error near unexpected token `|'\n");
			return (1);
		}
		if (ft_strncmp(pos, "&&&", 3) == 0)
		{
			ft_printf("syntax error near unexpected token `&'\n");
			return (1);
		}
		pos++;
	}
	return (0);
}

void	print_multiline_error(char *input)
{
	if (check_unclosed_quotes(input))
		ft_printf("syntax error: unterminated quoted string\n");
	else
		ft_printf("syntax error: unexpected end of file\n");
}
