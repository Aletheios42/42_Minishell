/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elorente <elorente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 13:51:35 by elorente          #+#    #+#             */
/*   Updated: 2025/05/25 13:51:35 by elorente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Inc/minishell.h"
#include "../libft/libft.h"

// Print a token's type as a string
const char	*get_token_type_string(t_token_type type)
{
	static const char	*token_names[] = {
		"REDIR_IN", "REDIR_OUT", "APPEND", "HEREDOC",
		"AND", "OR", "PIPE", "PAREN_OPEN", "PAREN_CLOSE",
		"WORD", "LITERAL_WORD", "EOF"
	};

	if (type >= 0 && type <= TOKEN_EOF)
		return (token_names[type]);
	return ("UNKNOWN");
}

// Helper function to print a token list with types
int	ft_print_token_list(t_token *head)
{
	t_token		*current;
	int			count;
	const char	*type_str;

	current = head;
	count = 0;
	count += ft_printf("[");
	while (current)
	{
		type_str = get_token_type_string(current->type);
		count += ft_printf("{\"%s\":%s}", current->value, type_str);
		if (current->next)
			count += ft_printf(", ");
		current = current->next;
	}
	count += ft_printf("]");
	return (count);
}

// Print detailed token information
void	print_token_list(t_token *head)
{
	t_token	*current;
	int		i;

	i = 0;
	current = head;
	if (!head)
	{
		ft_printf("(empty token list)\n");
		return ;
	}
	ft_printf("Token List:\n");
	while (current)
	{
		ft_printf("[%d] Type: %-15s Value: \"%s\"\n",
			i++, get_token_type_string(current->type), current->value);
		current = current->next;
	}
}
