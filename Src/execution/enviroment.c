/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enviroment.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alepinto <alepinto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 20:51:55 by elorente          #+#    #+#             */
/*   Updated: 2025/05/25 20:51:55 by elorente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Inc/minishell.h"
#include "../../libft/libft.h"

// ========== ENVIRONMENT VALUE ACCESS ==========

char	get_env_value(t_env *env, const char *key)
{
	if (!key)
		return (NULL);
	while (env)
	{
		if (env->key && ft_strcmp(env->key, key) == 0)
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

int	env_key_exists(t_env *env, const char *key)
{
	if (!key)
		return (0);
	while (env)
	{
		if (env->key && ft_strcmp(env->key, key) == 0)
			return (1);
		env = env->next;
	}
	return (0);
}

// ========== ENVIRONMENT MODIFICATION ==========

static	t_env	*create_env_node(const char *key, const char *value)
{
	t_env	*node;

	node = malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	node->key = ft_strdup(key);
	if (value != NULL)
		node->value = ft_strdup(value);
	else
		node->value = NULL;
	node->next = NULL;
	if (!node->key || (value != NULL && !node->value))
	{
		free(node->key);
		free(node->value);
		free(node);
		return (NULL);
	}
	return (node);
}

void	update_env_value(t_env *entry, const char *value)
{
	free(entry->value);
	if (value != NULL)
		entry->value = ft_strdup(value);
	else
		entry->value = NULL;
}

void	env_set_value(t_env **env, const char *key, const char *value)
{
	t_env	*current;
	t_env	*new_node;

	if (!env || !key)
		return ;
	current = *env;
	while (current)
	{
		if (current->key && ft_strcmp(current->key, key) == 0)
		{
			update_env_value(current, value);
			return ;
		}
		if (!current->next)
			break ;
		current = current->next;
	}
	new_node = create_env_node(key, value);
	if (!new_node)
		return ;
	if (!*env)
		*env = new_node;
	else
		current->next = new_node;
}
