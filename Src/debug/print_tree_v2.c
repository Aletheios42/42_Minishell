/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_tree_v2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elorente <elorente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 14:53:20 by elorente          #+#    #+#             */
/*   Updated: 2025/05/25 14:53:20 by elorente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Inc/minishell.h"
#include "../libft/libft.h"

int	p_operator(char	*label, t_token *token)
{
	if (!label)
		return (0);
	if (!token)
		return (ft_printf("%s", label));
	return (ft_printf("%s (%s)", label, token->value));
}

// Helper function in print tree node
int	print_tree_type_info(t_tree *tree)
{
	int	count;

	count = 0;
	if (tree->type == NODE_COMMAND)
	{
		count += ft_printf("COMMAND: ");
		if (tree->tokens)
		{
			count += ft_printf("TOKENS: ");
			count += ft_print_token_list(tree ->tokens);
		}
		else
			count += ft_printf("(empty)");
	}
	else if (tree->type == NODE_PIPE)
		count += p_operator("PIPE", tree->tokens);
	else if (tree->type == NODE_AND)
		count += p_operator("AND", tree->tokens);
	else if (tree->type == NODE_OR)
		count += p_operator("OR", tree->tokens);
	else if (tree->type == NODE_PAREN)
		count += p_operator("PIPE", tree->tokens);
	else
		count += ft_printf("UNKNOWN");
	return (count);
}
