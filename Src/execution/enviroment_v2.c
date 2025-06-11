/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enviroment_v2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elorente <elorente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 21:02:49 by elorente          #+#    #+#             */
/*   Updated: 2025/06/11 17:27:11 by elorente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Inc/minishell.h"
#include "../libft/libft.h"

void	env_unset_key(t_env **env, const char *key)
{
	t_env	*current;
	t_env	*prev;

	if (!env || !*env || !key)
		return ;
	current = *env;
	prev = NULL;
	while (current)
	{
		if (current->key && ft_strcmp(current->key, key) == 0)
		{
			if (prev)
				prev->next = current->next;
			else
				*env = current->next;
			free(current->key);
			free(current->value);
			free(current);
			return ;
		}
		prev = current;
		current = current->next;
	}
}

// ========== ASSIGNMENT PARSING ==========

int	parse_assignment(const char *assignment, char **key, char **value)
{
	char	*equal_pos;

	if (!assignment || !key || !value)
		return (0);
	*key = NULL;
	*value = NULL;
	equal_pos = ft_strchr(assignment, '=');
	if (!equal_pos)
		return (0);
	*key = ft_substr(assignment, 0, equal_pos - assignment);
	if (!*key)
		return (0);
	if (!is_valid_identifier(*key))
	{
		free(*key);
		*key = NULL;
		return (0);
	}
	*value = ft_strdup(equal_pos + 1);
	if (!*value)
		return (free(*key), *key = NULL, 0);
	return (1);
}

int	is_valid_identifier(const char *str)
{
	int	i;

	if (!str || !*str)
		return (0);
	if (!ft_isalpha(str[0]) && str[0] != '_')
		return (0);
	i = 1;
	while (str[i])
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

int	is_assignment(const char *str)
{
	char	*key;
	char	*value;
	int		result;

	if (!str)
		return (0);
	result = parse_assignment(str, &key, &value);
	free(key);
	free(value);
	return (result);
}

// ========== LEGACY FUNCTIONS FOR CD_V2 COMPATIBILITY ==========

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
