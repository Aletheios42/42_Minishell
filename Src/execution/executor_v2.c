/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_v2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elorente <elorente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 15:33:42 by elorente          #+#    #+#             */
/*   Updated: 2025/05/26 15:33:42 by elorente         ###   ########.fr       */
/*                                                                         
   */
/* ************************************************************************** */

#include "../Inc/minishell.h"
#include "../Inc/signals.h"
#include "../libft/libft.h"

int	execute_export_builtin(char **args, t_env **env)
{
	return (ft_export(args, *env, *env));
}

int	handle_exit_result(char **args)
{
	int	result;

	result = ft_exit(args);
	if (result == 0)
		return (1000);
	if (result == 1)
		return (1001);
	if (result == 255)
		return (1255);
	return (1000 + (result % 256));
}

int	execute_builtin_command(char **args, t_env **env)
{
	if (!args || !args[0])
		return (1);
	if (ft_strcmp(args[0], "echo") == 0)
		return (execute_echo_builtin(args));
	if (ft_strcmp(args[0], "cd") == 0)
		return (execute_cd_builtin(args, env));
	if (ft_strcmp(args[0], "pwd") == 0)
		return (ft_pwd(*env));
	if (ft_strcmp(args[0], "export") == 0)
		return (execute_export_builtin(args, env));
	if (ft_strcmp(args[0], "unset") == 0)
		return (ft_unset(args, env));
	if (ft_strcmp(args[0], "env") == 0)
		return (ft_env(*env));
	if (ft_strcmp(args[0], "exit") == 0)
		return (handle_exit_result(args));
	return (1);
}

// ========== TOKEN TO ARGS CONVERSION ==========

int	should_include_token(t_token *token, t_token *prev)
{
	if (!token || !token->value || token->value[0] == '\0')
		return (0);
	if (token->type != TOKEN_WORD && token->type != TOKEN_LITERAL_WORD)
		return (0);
	if (prev && (prev->type == TOKEN_REDIR_IN || prev->type == TOKEN_REDIR_OUT
			|| prev->type == TOKEN_APPEND || prev->type == TOKEN_HEREDOC))
		return (0);
	return (1);
}

int	count_word_tokens(t_token *tokens)
{
	t_token	*current;
	t_token	*prev;
	int		count;

	count = 0;
	current = tokens;
	prev = NULL;
	while (current)
	{
		if (should_include_token(current, prev))
			count++;
		prev = current;
		current = current->next;
	}
	return (count);
}
