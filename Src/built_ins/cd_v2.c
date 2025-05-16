/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_v2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elorente <elorente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 16:47:46 by elorente          #+#    #+#             */
/*   Updated: 2025/05/16 16:47:46 by elorente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Inc/minishell.h"

char	*get_env_value(t_env *env, const char *key)
{
	int	key_len;

	if (!env || !key)
		return (NULL);
	key_len = ft_strlen(key);
	while (env)
	{
		if (ft_strncmp(env->value, key, key_len) == 0 && env->value[key_len] == '=')
			return (env->value + key_len + 1);
		env = env->next;
	}
	return (NULL);
}

void	env_update(const char *key, const char *value, t_env *env)
{
	char	*new_var;
	char	curr_name[BUFF_SIZE];
	char	env_name[BUFF_SIZE];

	while (env)
	{
		get_env_name(curr_name, env->value);
		if (ft_strcmp(curr_name, key) == 0)
		{
			free(env->value);
			new_var = ft_strjoin(key, "=");
			env->value = ft_strjoin_free(new_var, value);
			return ;
		}
		env = env->next;
	}
	new_var = ft_strjoin(key, "=");
	new_var = ft_strjoin_free(new_var, value);
	env_add(new_var, env);
	free(new_var);
}

char	*ft_strjoin_free(char *s1, const char *s2)
{
	char	*res;
	res = ft_strjoin(s1, s2);
	free(s1);
	return (res);
}
