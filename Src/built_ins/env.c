/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elorente <elorente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 17:53:29 by elorente          #+#    #+#             */
/*   Updated: 2025/05/01 17:53:29 by elorente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Inc/minishell.h"

int	ft_env(t_env *env)
{
	while (env)
	{
		if (ft_strchr(env->value, '='))
			ft_printf("%s\n", env->value);
		env = env->next;
	}
	return (0);
}
