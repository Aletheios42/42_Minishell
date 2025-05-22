/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_v2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elorente <elorente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 19:41:23 by elorente          #+#    #+#             */
/*   Updated: 2025/05/12 19:41:23 by elorente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Inc/minishell.h"

void	print_sorted_env(t_env *env)
{
	while (env)
	{
		if (ft_strchr(env->value, '='))
			ft_printf("declare -x %s\n", env->value);
		else
			ft_printf("declare -x %s=\"\"\n", env->value);
		env = env->next;
	}
}

int ft_export(char *args[], t_env *env, t_env *secret)
{
	int	i;
	int	status;
	int	error;

	i = 1;
	status = 0;
	if (!args[1])
		return (print_sorted_env(secret), 0);
	while (args[i])
	{
		if (args[i][0] == '=')
			error = -3;
		else
			error = is_valid_key(args[i]);
		if (!error)
			status = print_error(error, args[i]);
		else if (!is_in_env(env, args[i]))
		{
			env_add(args[i], env);
			env_add(args[i], secret);
		}
		i++;
	}
	return (status);
}
