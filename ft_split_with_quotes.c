/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_with_quotes.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzolotar <mzolotar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 09:44:13 by mzolotar          #+#    #+#             */
/*   Updated: 2025/03/06 11:40:44 by mzolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
* @brief 
*
* @param 
* @return 
*/

int count_tokens(const char *sts, t_metachars *meta)
{
	int count = 0;
	int i = 0;
	bool in_single_quote = false;
	bool in_double_quote = false;

	while (sts[i])
	{
		if (sts[i] == ' ' && !in_single_quote && !in_double_quote)
			i++;
		else
		{
			count++;
			while (sts[i] && (in_single_quote || in_double_quote || sts[i] != ' '))
			{
				update_quote_status(sts[i], meta);
				i++;
			}
		}
	}
	return count;
}
/**
* @brief 
*
* @param 
* @return 
*/

char *extract_token(const char *sts, t_metachars *meta)
{
	int start = meta->i_current_index;
	int length = 0;
	bool in_single_quote = false;
	bool in_double_quote = false;

	while (sts[meta->i_current_index])
	{
		if (sts[meta->i_current_index] == '\'' && !in_double_quote)
			in_single_quote = !in_single_quote;
		else if (sts[meta->i_current_index] == '\"' && !in_single_quote)
			in_double_quote = !in_double_quote;
		else if (sts[meta->i_current_index] == ' ' && !in_single_quote && !in_double_quote)
			break;
		meta->i_current_index++;
		length++;
	}

	char *token = malloc(length + 1);
	if (!token)
		return (NULL);
	strncpy(token, &sts[start], length);
	token[length] = '\0';
	return token;
}
/**
* @brief 
*
* @param 
* @return 
*/

char **ft_split_with_quotes(char const *sts, t_metachars *meta)
{
	char **result;
	int count;

	if (!sts || !meta)
		return (NULL);
	meta->i_current_index = 0;
	count = count_tokens(sts, meta);
	result = malloc((count + 1) * sizeof(char *));
	if (!result)
		return (NULL);
	meta->j_new_index = 0;
	while (sts[meta->i_current_index])
	{
		if (sts[meta->i_current_index] == ' ' && !meta->in_single_quote && !meta->in_double_quote)
			meta->i_current_index++;
		else
			result[meta->j_new_index++] = extract_token(sts, meta);
	}
	result[meta->j_new_index] = NULL;
	return (result);
}
