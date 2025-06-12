/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elorente <elorente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 17:11:19 by elorente          #+#    #+#             */
/*   Updated: 2025/06/12 17:29:53 by elorente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Inc/minishell.h"
#include <stdbool.h>

int	is_n_flag(const char *str)
{
	int	i;

	if (str[0] != '-')
		return (0);
	i = 1;
	while (str[i] == 'n')
		i++;
	return (str[i] == '\0');
}

int	ft_echo(char *args[])
{
	int		i;
	bool	add_newline;

	if (!args)
		return (1);
	i = 1;
	add_newline = true;
	while (args[i] && is_n_flag(args[i]))
	{
		add_newline = false;
		i++;
	}
	while (args[i])
	{
		ft_printf("%s", args[i]);
		if (args[i + 1])
			ft_printf(" ");
		i++;
	}
	if (add_newline)
		ft_printf("\n");
	return (0);
}
