/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alepinto <alepinto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 15:33:56 by alepinto          #+#    #+#             */
/*   Updated: 2025/05/26 15:33:56 by alepinto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Inc/minishell.h"
#include "../Inc/signals.h"
#include "../libft/libft.h"

// ========== COMMAND TYPE DETECTION ==========

int	is_builtin_name(const char *cmd)
{
	if (ft_strcmp(cmd, "echo") == 0)
		return (1);
	if (ft_strcmp(cmd, "cd") == 0)
		return (1);
	if (ft_strcmp(cmd, "pwd") == 0)
		return (1);
	if (ft_strcmp(cmd, "export") == 0)
		return (1);
	if (ft_strcmp(cmd, "unset") == 0)
		return (1);
	if (ft_strcmp(cmd, "env") == 0)
		return (1);
	if (ft_strcmp(cmd, "exit") == 0)
		return (1);
	return (0);
}

char	*get_first_command_word(t_token *tokens)
{
	t_token	*current;
	t_token	*prev;

	current = tokens;
	prev = NULL;
	while (current)
	{
		if (current->type == TOKEN_WORD || current->type == TOKEN_LITERAL_WORD)
		{
			if (!prev || (prev->type != TOKEN_REDIR_IN
					&& prev->type != TOKEN_REDIR_OUT
					&& prev->type != TOKEN_APPEND
					&& prev->type != TOKEN_HEREDOC))
				return (current->value);
		}
		prev = current;
		current = current->next;
	}
	return (NULL);
}

t_command_type	classify_command(t_token *tokens, t_env *env)
{
	char	*first_word;
	char	*cmd_path;

	if (!tokens)
		return (CMD_INVALID);
	first_word = get_first_command_word(tokens);
	if (!first_word)
		return (CMD_INVALID);
	if (is_assignment(first_word))
		return (CMD_ASSIGNMENT);
	if (is_builtin_name(first_word))
		return (CMD_BUILTIN);
	cmd_path = find_command_in_path(first_word, env);
	if (cmd_path)
	{
		free(cmd_path);
		return (CMD_EXTERNAL);
	}
	return (CMD_INVALID);
}

// ========== BUILTIN EXECUTION ==========

int	execute_echo_builtin(char **args)
{
	return (ft_echo(args));
}

int	execute_cd_builtin(char **args, t_env **env)
{
	return (ft_cd(args, env));
}
