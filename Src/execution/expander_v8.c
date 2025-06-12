/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_v8.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elorente <elorente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 20:16:01 by elorente          #+#    #+#             */
/*   Updated: 2025/06/12 17:38:48 by elorente         ###   ########.fr       */
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

char	*expand_exit_status(char *res, int exit_status)
{
	char	*status_str;
	char	*tmp;

	status_str = ft_itoa(exit_status);
	if (!status_str)
		return (NULL);
	tmp = ft_strjoin(res, status_str);
	free(res);
	free(status_str);
	return (tmp);
}

char	*expand_string(const char *line, t_env *env, int exit_status)
{
	char			*res;
	t_expand_ctx	ctx;
	int				i;

	i = 0;
	if (res)
	{
		res = ft_strdup("");
		ctx.line = line;
		ctx.index = &i;
		ctx.env = env;
		ctx.exit_status = exit_status;
		if (!handle_dollar_loop(line, &res, &ctx))
		{
			free(res);
			return (NULL);
		}
		return (res);
	}
	return (NULL);
}
