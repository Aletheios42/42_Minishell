/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_v4.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elorente <elorente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 23:03:44 by alepinto          #+#    #+#             */
/*   Updated: 2025/06/09 17:40:13 by elorente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Inc/minishell.h"
#include <stdlib.h>
#include "../libft/libft.h"

int	validate_parentheses(t_token *tokens)
{
	int	paren_count;

	paren_count = 0;
	while (tokens)
	{
		if (tokens->type == TOKEN_PAREN_OPEN)
			paren_count++;
		else if (tokens->type == TOKEN_PAREN_CLOSE)
			paren_count--;
		if (paren_count < 0)
		{
			ft_printf("syntax error: unexpected ')'\n");
			return (0);
		}
		tokens = tokens->next;
	}
	if (paren_count != 0)
	{
		ft_printf("syntax error: unmatched '('\n");
		return (0);
	}
	return (1);
}

int	validate_operators(t_token *tokens)
{
	t_token	*prev;

	prev = NULL;
	while (tokens)
	{
		if (prev
			&& (prev->type == TOKEN_PIPE || prev->type == TOKEN_AND
				|| prev->type == TOKEN_OR)
			&& (tokens->type == TOKEN_PIPE || tokens->type == TOKEN_AND
				|| tokens->type == TOKEN_OR))
		{
			ft_printf("syntax error near unexpected token `%s'\n",
				tokens->value);
			return (0);
		}
		prev = tokens;
		tokens = tokens->next;
	}
	return (1);
}

int	validate_redirections(t_token *tokens)
{
	while (tokens)
	{
		if (tokens->type == TOKEN_REDIR_IN || tokens->type == TOKEN_REDIR_OUT
			|| tokens->type == TOKEN_APPEND || tokens->type == TOKEN_HEREDOC)
		{
			if (!tokens->next
				|| (tokens->next->type != TOKEN_WORD
					&& tokens->next->type != TOKEN_LITERAL_WORD))
			{
				ft_printf("syntax error near unexpected token `newline'\n");
				return (0);
			}
		}
		tokens = tokens->next;
	}
	return (1);
}

int	validate_syntax(t_token *tokens)
{
	if (!validate_parentheses(tokens))
		return (0);
	if (!validate_operators(tokens))
		return (0);
	if (!validate_redirections(tokens))
		return (0);
	return (1);
}

// ========== PUBLIC INTERFACE ==========

t_tree	*parser(t_token *tokens)
{
	if (!tokens || !tokens->value || *tokens->value == '\0')
	{
		ft_putendl_fd("minishell: empty input or invalid token", 2);
		return (NULL);
	}
	if (!validate_syntax(tokens))
		return (NULL);
	return (parse_expression(tokens));
}
