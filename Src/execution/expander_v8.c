/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_v8.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elorente <elorente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 20:16:01 by elorente          #+#    #+#             */
/*   Updated: 2025/06/09 21:17:53 by elorente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Inc/minishell.h"
#include "../libft/libft.h"
#include <dirent.h>
#include <sys/stat.h>

t_token	*expand_token_list_copy(t_token *original, t_env *env, int status)
{
	t_token	*head;
	t_token	*tail;
	t_token	*expanded;

	head = NULL;
	tail = NULL;
	while (original)
	{
		expanded = expand_and_split_token_copy(original, env, status);
		handle_expanded_token(expanded, &head, &tail);
		original = original->next;
	}
	if (!head)
	{
		ft_putendl_fd("minishell: no arguments after expansion", 2);
		return (NULL);
	}
	return (head);
}

int	should_expand_segment(int i, int start)
{
	return (i > start);
}

int	expand_and_append(char **res, const char *line, int *start, int i)
{
	char	*seg;
	char	*new_res;

	seg = ft_substr(line, *start, i - *start);
	if (!seg)
		return (0);
	new_res = join_and_free(*res, seg);
	if (!new_res)
		return (0);
	*res = new_res;
	return (1);
}

int	handle_dollar_loop(const char *line, char **res, t_expand_ctx *ctx)
{
	int		i;
	int		start;

	i = 0;
	start = 0;
	while (line[i])
	{
		if (line[i] == '$')
		{
			if (i > start && !expand_and_append(res, line, &start, i))
				return (0);
			*res = expand_variable_segment(*res, ctx);
			if (!*res)
				return (0);
			start = i;
		}
		else
			i++;
	}
	if (i > start && !expand_and_append(res, line, &start, i))
		return (0);
	return (1);
}

char	*expand_string(const char *line, t_env *env, int exit_status)
{
	char			*res;
	t_expand_ctx	ctx;
	int				i;

	i = 0;
	res = ft_strdup("");
	if (!res)
		return (NULL);
	ctx.line = line;
	ctx.index = &i;
	ctx.env = env;
	ctx.exit_status = exit_status;
	if (!handle_dollar_loop(line, &res, &ctx))
		return (NULL);
	return (res);
}
