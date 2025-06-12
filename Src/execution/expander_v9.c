/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_v9.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elorente <elorente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 15:55:24 by elorente          #+#    #+#             */
/*   Updated: 2025/06/12 16:16:50 by elorente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Inc/minishell.h"
#include "../libft/libft.h"
#include <dirent.h>
#include <sys/stat.h>

int	handle_dollar_segment(t_dollar_loop_data *d)
{
	if (d->line[d->i] == '$')
	{
		if (d->i > d->start
			&& !expand_and_append(d->res, d->line, &d->start, d->i))
			return (0);
		if (d->line[d->i + 1] == '?')
		{
			*(d->res) = expand_exit_status(*(d->res), d->ctx->exit_status);
			if (!*(d->res))
				return (0);
			d->i += 2;
		}
		else
		{
			d->ctx->index = &(d->i);
			*(d->res) = expand_variable_segment(*(d->res), d->ctx);
			if (!*(d->res))
				return (0);
			d->i = *(d->ctx->index);
		}
		d->start = d->i;
	}
	else
		d->i++;
	return (1);
}

int	handle_dollar_loop(const char *line, char **res, t_expand_ctx *ctx)
{
	t_dollar_loop_data	d;

	d.i = 0;
	d.start = 0;
	d.line = line;
	d.res = res;
	d.ctx = ctx;
	while (d.line[d.i])
	{
		if (!handle_dollar_segment(&d))
			return (0);
	}
	if (d.i > d.start && !expand_and_append(d.res, d.line, &d.start, d.i))
		return (0);
	return (1);
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
