/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_v7.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elorente <elorente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 00:22:07 by elorente          #+#    #+#             */
/*   Updated: 2025/06/11 19:53:41 by elorente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Inc/minishell.h"
#include "../libft/libft.h"
#include <dirent.h>
#include <sys/stat.h>

char	*build_path(const char *dir_path, const char *filename)
{
	char	*temp;
	char	*full;

	if (ft_strcmp(dir_path, ".") == 0)
		return (ft_strdup(filename));
	temp = ft_strjoin(dir_path, "/");
	if (!temp)
		return (NULL);
	full = ft_strjoin(temp, filename);
	free(temp);
	return (full);
}

char	*append_variable_expansion(char *result, t_expand_ctx *ctx)
{
	char	*var_name;
	char	*var_value;
	int		consumed;

	var_name = extract_variable_name(&ctx->line[*(ctx->index)], &consumed);
	var_value = resolve_variable_value(var_name, ctx->env, ctx->exit_status);
	free(var_name);
	*(ctx->index) += consumed;
	return (join_and_free(result, var_value));
}

char	*append_raw_segment(char *result, const char *line, int end)
{
	char	*segment;

	segment = ft_substr(line, 0, end);
	result = join_and_free(result, segment);
	return (result);
}
/*
char	*expand_variable_segment(char *result, t_expand_ctx *ctx)
{
	char	*var_name;
	char	*value;
	int		consumed;

	(*(ctx->index))++;
	consumed = 0;
	var_name = extract_variable_name(&(ctx->line[*(ctx->index)]), &consumed);
	if (!var_name)
		return (result);
	*(ctx->index) += consumed;
	value = get_env_value(ctx->env, var_name);
	free(var_name);
	if (!value)
		value = "";
	char *expanded = ft_strdup(value);
	if (!expanded)
		return (NULL);
	result = join_and_free(result, expanded);
	return (result);
}
*/
/*
char	*expand_variable_segment(char *result, t_expand_ctx *ctx)
{
	char	*var_name;
	char	*value;
	char	*tmp;
	int		consumed;

	(*(ctx->index))++;
	consumed = 0;
	var_name = extract_variable_name(&(ctx->line[*(ctx->index)]), &consumed);
	if (!var_name)
		return (result);
	*(ctx->index) += consumed;
	value = get_env_value(ctx->env, var_name);
	free(var_name);
	if (!value)
		result = join_and_free(result, ft_strdup(""));
	else
		result = join_and_free(result, ft_strdup(value));
	if (!result)
		return (NULL);
	ctx->line = &(ctx->line[*(ctx->index)]);
	*(ctx->index) = 0;
	return (result);
}
*/

char	*expand_variable_segment(char *result, t_expand_ctx *ctx)
{
	char	*var_name;
	char	*value;
	char	*tmp;
	int		consumed;

	(*(ctx->index))++;
	consumed = 0;
	var_name = extract_variable_name(&(ctx->line[*(ctx->index)]), &consumed);
	if (!var_name)
		return (result);
	*(ctx->index) += consumed;
	value = get_env_value(ctx->env, var_name);
	free(var_name);

	if (value)
	{
		tmp = ft_strdup(value);
		if (!tmp)
			return (NULL);
		result = join_and_free(result, tmp);
		if (!result)
			return (NULL);
	}
	return (result);
}


char	*append_segment(char *res, const char *line, int start, int end)
{
	char	*seg;
	char	*new_res;

	seg = ft_substr(line, start, end - start);
	if (!seg)
	{
		free(res);
		return (NULL);
	}
	new_res = join_and_free(res, seg);
	return (new_res);
}
