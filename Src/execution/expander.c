/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elorente <elorente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 19:52:41 by elorente          #+#    #+#             */
/*   Updated: 2025/05/27 18:06:54 by elorente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Inc/minishell.h"
#include "../libft/libft.h"
#include <dirent.h>
#include <sys/stat.h>

// ========== VARIABLE NAME EXTRACTION ==========

char	*extract_variable_name(const char *str, int *consumed_len)
{
	int	i;

	if (!str || !consumed_len)
		return (NULL);
	*consumed_len = 0;
	if (str[0] == '?')
		return (*consumed_len = 1, ft_strdup("?"));
	if (str[0] == '{')
	{
		i = 1;
		while (str[i] && str[i] != '}')
			i++;
		if (str[i] == '}')
			return (*consumed_len = i + 1, ft_substr(str, 1, i - 1));
		return (NULL);
	}
	i = 0;
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		i++;
	if (i == 0)
		return (NULL);
	*consumed_len = i;
	return (ft_substr(str, 0, i));
}

// ========== VARIABLE VALUE RESOLUTION ==========

char	*resolve_variable_value(const char *var_name, t_env *env, int exit)
{
	char	*value;

	if (!var_name)
		return (ft_strdup(""));
	if (ft_strcmp(var_name, "?") == 0)
		return (ft_itoa(exit));
	value = get_env_value(env, var_name);
	if (value)
		return (ft_strdup(value));
	return (ft_strdup(""));
}

// ========== STRING EXPANSION ==========

char	*join_and_free(char *s1, char *s2)
{
	char	*joined;

	if (!s2)
		joined = ft_strjoin(s1, "");
	else
		joined = ft_strjoin(s1, s2);
	free(s1);
	if (s2)
		free(s2);
	return (joined);
}

char	*append_char_and_free(char *str, char c)
{
	char	tmp[2];
	char	*res;

	tmp[0] = c;
	tmp[1] = '\0';
	res = ft_strjoin(str, tmp);
	free(str);
	return (res);
}
