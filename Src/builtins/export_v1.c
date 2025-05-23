/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_v1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elorente <elorente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 19:04:17 by elorente          #+#    #+#             */
/*   Updated: 2025/05/12 19:04:17 by elorente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Inc/minishell.h"

// ========== ENVIRONMENT DISPLAY ==========

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

// ========== MAIN EXPORT FUNCTION ==========

int	process_export_argument(char *arg, t_env **env, t_env **secret)
{
	char	*key;
	char	*value;

	printf("DEBUG EXPORT: arg='%s'\n", arg);  // DEBUG
	
	if (arg[0] == '=')
	{
		ft_printf("minishell: export: '%s': not a valid identifier\n", arg);
		return (1);
	}
	if (is_assignment(arg))
	{
		if (parse_assignment(arg, &key, &value))
		{
			printf("DEBUG PARSE: key='%s', value='%s'\n", key, value);  // DEBUG
			env_set_value(env, key, value);
			env_set_value(secret, key, value);
			free(key);
			free(value);
			return (0);
		}
		else
		{
			ft_printf("minishell: export: '%s': not a valid identifier\n", arg);
			return (1);
		}
	}
	else if (is_valid_identifier(arg))
	{
		env_set_value(env, arg, NULL);
		env_set_value(secret, arg, NULL);
		return (0);
	}
	else
	{
		ft_printf("minishell: export: '%s': not a valid identifier\n", arg);
		return (1);
	}
}

int	ft_export(char *args[], t_env *env, t_env *secret)
{
	int	i;
	int	status;
	int	arg_status;

	if (!args[1])
	{
		print_sorted_env(secret);
		return (0);
	}
	i = 1;
	status = 0;
	while (args[i])
	{
		arg_status = process_export_argument(args[i], &env, &secret);
		if (arg_status != 0)
			status = 1;
		i++;
	}
	return (status);
}
