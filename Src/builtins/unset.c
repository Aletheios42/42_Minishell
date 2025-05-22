/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elorente <elorente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 20:33:47 by elorente          #+#    #+#             */
/*   Updated: 2025/05/23 15:30:00 by elorente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Inc/minishell.h"

void	remove_env_var(t_env *env[], const char *key)
{
	t_env	*curr;
	t_env	*prev;
	char	var_name[BUFF_SIZE];
	char	curr_name[BUFF_SIZE];

	curr = *env;
	prev = NULL;
	get_env_name(var_name, key);
	while (curr)
	{
		get_env_name(curr_name, curr->value);
		if (ft_strcmp(var_name, curr_name) == 0)
		{
			if (prev)
				prev->next = curr->next;
			else
				*env = curr->next;
			free(curr->value);
			free(curr);
			return ;
		}
		prev = curr;
		curr = curr->next;
	}
}

int	ft_unset(char *args[], t_env *env[], t_local_vars **locals)
{
	int	i;

	i = 1;
	if (!args[i])
		return (0);
	while (args[i])
	{
		if (!is_valid_key(args[i]))
		{
			ft_putstr_fd("minishell: unset: '", 2);
			ft_putstr_fd(args[i], 2);
			ft_putendl_fd("': not a valid identifier", 2);
		}
		else
		{
			remove_env_var(env, args[i]);
			local_var_unset(locals, args[i]);
		}
		i++;
	}
	return (0);
}
