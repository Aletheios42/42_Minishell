/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enviroment_v4.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elorente <elorente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 14:42:35 by elorente          #+#    #+#             */
/*   Updated: 2025/05/26 14:42:35 by elorente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Inc/minishell.h"
#include "../libft/libft.h"

char	*build_env_string(const char *key, const char *value)
{
	char	*temp;
	char	*env_string;

	if (!key)
		return (NULL);
	if (value)
	{
		temp = ft_strjoin(key, "=");
		if (!temp)
			return (NULL);
		env_string = ft_strjoin(temp, value);
		free(temp);
		return (env_string);
	}
	return (ft_strjoin(key, "="));
}

char	**env_to_array(t_env *env)
{
	char	**envp;
	char	*env_string;
	int		count;
	int		i;

	count = count_env_vars(env);
	envp = malloc(sizeof(char *) * (count + 1));
	if (!envp)
		return (NULL);
	i = 0;
	while (env && i < count)
	{
		if (env->key)
		{
			env_string = build_env_string(env->key, env->value);
			if (!env_string)
				return (ft_free_matrix(envp), NULL);
			envp[i++] = env_string;
		}
		env = env->next;
	}
	envp[i] = NULL;
	return (envp);
}

// ========== ENVIRONMENT CLEANUP ==========

void	free_env_list(t_env *head)
{
	t_env	*current;
	t_env	*next;

	current = head;
	while (current)
	{
		next = current->next;
		free(current->key);
		free(current->value);
		free(current);
		current = next;
	}
}

// ========== ENVIRONMENT UTILITIES ==========

void	print_env_list(t_env *env)
{
	while (env)
	{
		if (env->key)
		{
			ft_printf("%s", env->key);
			if (env->value)
				ft_printf("=%s", env->value);
			ft_printf("\n");
		}
		env = env->next;
	}
}

t_env	*copy_env_list(t_env *original)
{
	t_env	*head;
	t_env	*current;
	t_env	*new_node;

	if (!original)
		return (NULL);
	head = NULL;
	current = NULL;
	while (original)
	{
		if (original->key)
		{
			new_node = create_env_node(original->key, original->value);
			if (!new_node)
				return (free_env_list(head), NULL);
			if (!head)
				head = new_node;
			else
				current->next = new_node;
			current = new_node;
		}
		original = original->next;
	}
	return (head);
}
