/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enviroment_v3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alepinto <alepinto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 13:52:27 by alepinto          #+#    #+#             */
/*   Updated: 2025/05/26 13:52:27 by alepinto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Inc/minishell.h"
#include "../libft/libft.h"

void	env_add(const char *value, t_env *env)
{
	t_env	*new_node;

	if (!value || !env)
		return ;
	while (env && env->next)
		env = env->next;
	new_node = malloc(sizeof(t_env));
	if (!new_node)
		return ;
	new_node->key = NULL;
	new_node->value = ft_strdup(value);
	new_node->next = NULL;
	env->next = new_node;
}

// ========== ENVIRONMENT INITIALIZATION ==========

t_env	*create_env_node(const char *key, const char *value)
{
	t_env	*node;

	node = malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	if (key)
		node->key = ft_strdup(key);
	else
		node->key = NULL;
	if (value)
		node->value = ft_strdup(value);
	else
		node->value = NULL;
	node->next = NULL;
	if ((key && !node->key) || (value && !node->value))
	{
		free(node->key);
		free(node->value);
		free(node);
		return (NULL);
	}
	return (node);
}

t_env	*create_env_node_from_string(const char *env_str)
{
	char	*key;
	char	*value;
	t_env	*node;

	if (!env_str)
		return (NULL);
	if (!parse_assignment(env_str, &key, &value))
		return (NULL);
	node = create_env_node(key, value);
	free(key);
	free(value);
	return (node);
}

t_env	*env_from_array(char **envp)
{
	t_env	*head;
	t_env	*current;
	t_env	*new_node;
	int		i;

	if (!envp)
		return (NULL);
	head = NULL;
	current = NULL;
	i = 0;
	while (envp[i])
	{
		new_node = create_env_node_from_string(envp[i]);
		if (!new_node)
			return (free_env_list(head), NULL);
		if (!head)
			head = new_node;
		else
			current->next = new_node;
		current = new_node;
		i++;
	}
	return (head);
}

// ========== ENVIRONMENT CONVERSION ==========

int	count_env_vars(t_env *env)
{
	int	count;

	count = 0;
	while (env)
	{
		if (env->key)
			count++;
		env = env->next;
	}
	return (count);
}
