/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elorente <elorente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 17:11:19 by elorente          #+#    #+#             */
/*   Updated: 2025/06/09 17:43:42 by elorente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Inc/minishell.h"

int	ft_echo(char *args[])
{
	int	i;
	int	add_newline;

	if (!args)
		return (1);
	i = 1;
	add_newline = 1;
	if (args[i] && ft_strcmp(args[i], "-n") == 0)
	{
		add_newline = 0;
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
