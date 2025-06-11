/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elorente <elorente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 19:52:41 by elorente          #+#    #+#             */
/*   Updated: 2025/06/11 17:26:59 by elorente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Inc/minishell.h"
#include "../libft/libft.h"
#include <dirent.h>
#include <sys/stat.h>

// ========== VARIABLE NAME EXTRACTION ==========

char	*return_var_segment(const char *str, int i, int *consumed_len)
{
	if (i == 0)
	{
		*consumed_len = 0;
		return (ft_strdup(""));
	}
	*consumed_len = i;
	return (ft_substr(str, 0, i));
}

static char	*empty_var_name(int *consumed_len)
{
	if (consumed_len)
		*consumed_len = 0;
	return (ft_strdup(""));
}

char	*extract_variable_name(const char *str, int *consumed_len)
{
	int	i;

	if (!str || !consumed_len)
		return (empty_var_name(NULL));
	if (str[0] == '?')
	{
		*consumed_len = 1;
		return (ft_strdup("?"));
	}
	if (str[0] == '{')
	{
		i = 1;
		while (str[i] && str[i] != '}')
			i++;
		if (str[i] != '}')
			return (empty_var_name(consumed_len));
		*consumed_len = i + 1;
		return (ft_substr(str, 1, i - 1));
	}
	i = 0;
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		i++;
	if (i == 0)
		return (empty_var_name(consumed_len));
	*consumed_len = i;
	return (ft_substr(str, 0, i));
}

/*
char	*extract_variable_name(const char *str, int *consumed_len)
{
	int	i;

	if (!str || !consumed_len)
		return (ft_strdup(""));
	if (str[0] == '?')
	{
		*consumed_len = 1;
		return (ft_strdup("?"));
	}
	if (str[0] == '{')
	{
		i = 1;
		while (str[i] && str[i] != '}')
			i++;
		if (str[i] != '}')
			return (ft_strdup(""));
		*consumed_len = i + 1;
		return (ft_substr(str, 1, i - 1));
	}
	i = 0;
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		i++;
	return (return_var_segment(str, i, consumed_len));
}*/

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
