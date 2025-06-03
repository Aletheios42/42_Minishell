/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elorente <elorente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 22:42:32 by elorente          #+#    #+#             */
/*   Updated: 2025/06/03 22:42:32 by elorente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "../Inc/minishell.h"
#include <readline/readline.h>
#include <readline/history.h>

// ========== METACHARACTER CLASSIFICATION ==========

int	is_ifs_char(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

int	is_logical_operator(char *str)
{
	return (ft_strncmp(str, "&&", 2) == 0
		|| ft_strncmp(str, "||", 2) == 0
		|| ft_strncmp(str, "|", 1) == 0
		|| ft_strncmp(str, ";", 1) == 0);
}

int	is_redirection_operator(char *str)
{
	return (ft_strncmp(str, "<<", 2) == 0
		|| ft_strncmp(str, ">>", 2) == 0
		|| ft_strncmp(str, "<", 1) == 0
		|| ft_strncmp(str, ">", 1) == 0);
}

int	is_parenthesis_operator(char *str)
{
	return (*str == '(' || *str == ')');
}

int	is_quote_char(char c)
{
	return (c == '"' || c == '\'');
}
