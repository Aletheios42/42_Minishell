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

#include "../Inc/minishell.h"

int	is_valid_key(const char *arg)
{
	int	i;

	i = 0;
	if (!arg || (!ft_isalpha(arg[0]) && arg[0] != '_'))
		return (0);
	while (arg[i] && arg[i] != '=')
	{
		if (!ft_isalnum(arg[i]) && arg[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

char	*get_env_name(char *dest, const char *src)
{
	int	i;

	i = 0;
	while (src[i] && src[i] != '=' && i < BUFF_SIZE - 1)
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

int	is_in_env(t_env *env, char *arg)
{
	char	var_name[BUFF_SIZE];
	char	env_name[BUFF_SIZE];

	get_env_name(var_name, arg);
	while (env)
	{
		get_env_name(env_name, env->value);
		if (ft_strcmp(var_name, env_name) == 0)
		{
			free(env->value);
			env->value = NULL;
			env->value = ft_strdup(arg);
			return (1);
		}
		env = env->next;
	}
	return (0);
}

void	env_add(const char *value, t_env *env)
{
	t_env	*new;

	if (!value)
		return ;
	while (env && env->next)
		env = env->next;
	new = malloc(sizeof(t_env));
	if (!new)
		return ;
	new->value = ft_strdup(value);
	new->next = NULL;
	env->next = new;
}

int	print_error(int error, const char *arg)
{
	ft_putstr_fd("minishell: export: '", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd("': ", 2);
	if (error == -3)
		ft_putendl_fd("invalid assigment", 2);
	else
		ft_putendl_fd("not a valid identifier", 2);
	return (1);
}