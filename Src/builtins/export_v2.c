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
		if (env->key)
		{
			ft_printf("declare -x %s", env->key);
			if (env->value)
				ft_printf("=\"%s\"", env->value);
			else
				ft_printf("=\"\"");
			ft_printf("\n");
		}
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
			error = 0;  // Invalid assignment
		else
			error = is_valid_identifier(args[i]) || is_assignment(args[i]);
		if (!error)
		{
			ft_putstr_fd("minishell: export: '", 2);
			ft_putstr_fd(args[i], 2);
			ft_putendl_fd("': not a valid identifier", 2);
			status = 1;
		}
		else if (is_assignment(args[i]))
		{
			// Handle assignment
			char *key;
			char *value;
			if (parse_assignment(args[i], &key, &value))
			{
				env_set_value(&env, key, value);
				env_set_value(&secret, key, value);
				free(key);
				free(value);
			}
		}
		else
		{
			// Just declare variable without value
			env_set_value(&env, args[i], NULL);
			env_set_value(&secret, args[i], NULL);
		}
		i++;
	}
	return (status);
}
