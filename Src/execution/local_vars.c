/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   local_vars.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alepinto <alepinto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 15:00:00 by alepinto          #+#    #+#             */
/*   Updated: 2025/05/23 15:00:00 by alepinto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Inc/minishell.h"

static t_local_vars	*create_local_var(char *key, char *value)
{
	t_local_vars	*new_var;

	new_var = malloc(sizeof(t_local_vars));
	if (!new_var)
		return (NULL);
	new_var->key = ft_strdup(key);
	new_var->value = ft_strdup(value);
	new_var->next = NULL;
	if (!new_var->key || !new_var->value)
	{
		free(new_var->key);
		free(new_var->value);
		free(new_var);
		return (NULL);
	}
	return (new_var);
}

void	local_var_set(t_local_vars **locals, char *key, char *value)
{
	t_local_vars	*current;
	t_local_vars	*new_var;

	if (!locals || !key)
		return ;
	current = *locals;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			free(current->value);
			current->value = ft_strdup(value);
			return ;
		}
		if (!current->next)
			break ;
		current = current->next;
	}
	new_var = create_local_var(key, value);
	if (!new_var)
		return ;
	if (!*locals)
		*locals = new_var;
	else
		current->next = new_var;
}

char	*local_var_get(t_local_vars *locals, char *key)
{
	t_local_vars	*current;

	if (!locals || !key)
		return (NULL);
	current = locals;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
			return (current->value);
		current = current->next;
	}
	return (NULL);
}

void	local_var_unset(t_local_vars **locals, char *key)
{
	t_local_vars	*current;
	t_local_vars	*prev;

	if (!locals || !*locals || !key)
		return ;
	current = *locals;
	prev = NULL;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			if (prev)
				prev->next = current->next;
			else
				*locals = current->next;
			free(current->key);
			free(current->value);
			free(current);
			return ;
		}
		prev = current;
		current = current->next;
	}
}

void	free_local_vars(t_local_vars *locals)
{
	t_local_vars	*current;
	t_local_vars	*next;

	current = locals;
	while (current)
	{
		next = current->next;
		free(current->key);
		free(current->value);
		free(current);
		current = next;
	}
}
