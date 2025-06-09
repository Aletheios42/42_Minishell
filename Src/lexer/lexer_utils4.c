/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils4.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elorente <elorente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 23:20:21 by elorente          #+#    #+#             */
/*   Updated: 2025/06/09 20:02:34 by elorente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "../Inc/minishell.h"
#include <readline/readline.h>
#include <readline/history.h>

char	*handle_multiline_input(char *input)
{
	char	*line;
	char	*new_input;
	char	*temp;

	if (check_syntax_errors(input))
		return (free(input), NULL);
	while (check_unclosed_quotes(input) || check_unclosed_parens(input) > 0)
	{
		if (check_unclosed_quotes(input))
			line = readline("quote> ");
		else
			line = readline("paren> ");
		if (!line)
		{
			print_multiline_error(input);
			return (free(input), NULL);
		}
		temp = ft_strjoin(input, "\n");
		free(input);
		new_input = ft_strjoin(temp, line);
		free(temp);
		free(line);
		input = new_input;
	}
	return (input);
}

// ========== MAIN TOKENIZER ==========
int	add_current_token(t_string_builder *sb, t_token **head,
					t_token **tail, t_token_type token_type)
{
	char	*token_value;
	t_token	*new_token;

	if (!sb || sb->len == 0)
		return (1);
	token_value = finalize_string_builder(sb);
	if (!token_value)
		return (0);
	new_token = create_token(token_value, token_type);
	if (!new_token)
	{
		free(token_value);
		return (0);
	}
	add_token_to_list(head, tail, new_token);
	return (1);
}

int	add_operator_token(char *op, t_token **head, t_token **tail)
{
	t_token	*new_token;
	char	*op_value;

	op_value = ft_strdup(op);
	if (!op_value)
		return (0);
	new_token = create_token(op_value, get_operator_token_type(op));
	if (!new_token)
	{
		free(op_value);
		return (0);
	}
	add_token_to_list(head, tail, new_token);
	return (1);
}

// ========== PUBLIC INTERFACE ==========
t_token	*lexer(char *input)
{
	char	*processed_input;
	t_token	*tokens;

	if (!input)
		return (NULL);
	processed_input = ft_strdup(input);
	if (!processed_input)
		return (NULL);
	processed_input = handle_multiline_input(processed_input);
	if (!processed_input)
		return (NULL);
	tokens = tokenize_input(processed_input);
	free(processed_input);
	return (tokens);
}
