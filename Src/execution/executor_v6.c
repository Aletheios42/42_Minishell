/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_v6.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elorente <elorente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 19:21:24 by elorente          #+#    #+#             */
/*   Updated: 2025/05/26 19:21:24 by elorente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Inc/minishell.h"
#include "../Inc/signals.h"
#include "../libft/libft.h"

// ========== ASSIGNMENT EXECUTION ==========

int	execute_assignments(t_token *tokens, t_env **env)
{
	t_token		*current;
	int			assignment_count;
	char		*key;
	char		*value;
	char		*expanded_value;

	assignment_count = 0;
	current = tokens;
	while (current)
	{
		if (current->type == TOKEN_WORD && is_assignment(current->value))
		{
			if (parse_assignment(current->value, &key, &value))
			{
				expanded_value = expand_string(value, *env, 0);
				env_set_value(env, key, expanded_value);
				free(key);
				free(value);
				free(expanded_value);
				assignment_count++;
			}
		}
		current = current->next;
	}
	return (assignment_count);
}

// ========== COMMAND EXECUTION ==========

int	execute_simple_command(t_token *tokens, t_env **env, int exit_status)
{
	t_command_type		cmd_type;
	t_token				*expanded_tokens;
	char				**args;
	int					result;
	int					saved_stdin;
	int					saved_stdout;

	if (!tokens)
		return (0);
	execute_assignments(tokens, env);
	expanded_tokens = expand_token_list_copy(tokens, *env, exit_status);
	if (!expanded_tokens)
		return (1);
	cmd_type = classify_command(expanded_tokens, *env);
	if (cmd_type == CMD_ASSIGNMENT)
		return (free_token_list(expanded_tokens), 0);
	if (setup_redirections(expanded_tokens, &saved_stdin,
			&saved_stdout, *env, exit_status) == -1)
		return (free_token_list(expanded_tokens), 1);
	args = tokens_to_args_array(expanded_tokens);
	if (!args)
	{
		restore_redirections(saved_stdin, saved_stdout);
		return (ree_token_list(expanded_tokens), 0);
	}
	if (cmd_type == CMD_BUILTIN)
		result = execute_builtin_command(args, env);
	else if (cmd_type == CMD_EXTERNAL)
		result = execute_external_command(args, *env);
	else
	{
		ft_printf("minishell: %s: command not found\n", args[0]);
		result = 127;
	}
	restore_redirections(saved_stdin, saved_stdout);
	ft_free_matrix(args);
	free_token_list(expanded_tokens);
	return (result);
}

// ========== PIPELINE EXECUTION ==========

int	setup_pipeline_fds(int pipefd[2])
{
	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		return (-1);
	}
	return (0);
}

int	fork_left_process(t_tree *tree, t_env **env, int exit_status, int pipefd[2])
{
	pid_t	left_pid;

	left_pid = fork();
	if (left_pid == 0)
	{
		close(pipefd[0]);
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);
		exit(execute_tree(tree->left, env, exit_status));
	}
	return (left_pid);
}

int	fork_right_process(t_tree *tree, t_env **env,
		int exit_status, int pipefd[2])
{
	pid_t	right_pid;

	right_pid = fork();
	if (right_pid == 0)
	{
		close(pipefd[1]);
		dup2(pipefd[0], STDIN_FILENO);
		close(pipefd[0]);
		exit(execute_tree(tree->right, env, exit_status));
	}
	return (right_pid);
}
