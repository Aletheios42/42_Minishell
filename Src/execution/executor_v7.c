/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_v7.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alepinto <alepinto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 00:24:10 by alepinto          #+#    #+#             */
/*   Updated: 2025/06/05 00:24:10 by alepinto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Inc/minishell.h"
#include "../Inc/signals.h"
#include "../libft/libft.h"










int execute_pipeline(t_tree *tree, t_env **env, int exit_status)
{
	int pipefd[2];
	pid_t left_pid;
	pid_t right_pid;
	int status;
	int saved_stdin;
	int saved_stdout;
	
	if (!tree || !tree->left || !tree->right)
		return 1;
	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	if (setup_pipeline_fds(pipefd) == -1)
	{
		close(saved_stdin);
		close(saved_stdout);
		return 1;
	}
	left_pid = fork_left_process(tree, env, exit_status, pipefd);
	if (left_pid == -1)
	{
		perror("fork");
		close(pipefd[0]);
		close(pipefd[1]);
		close(saved_stdin);
		close(saved_stdout);
		return 1;
	}
	right_pid = fork_right_process(tree, env, exit_status, pipefd);
	if (right_pid == -1)
	{
		perror("fork");
		close(pipefd[0]);
		close(pipefd[1]);
		kill(left_pid, SIGTERM);
		waitpid(left_pid, NULL, 0);
		close(saved_stdin);
		close(saved_stdout);
		return 1;
	}
	close(pipefd[0]);
	close(pipefd[1]);
	waitpid(left_pid, NULL, 0);
	waitpid(right_pid, &status, 0);
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdin);
	close(saved_stdout);
	return WEXITSTATUS(status);
}
















// ========== LOGICAL OPERATIONS ==========

int execute_logical_and(t_tree *tree, t_env **env, int exit_status)
{
	int left_result;
	
	if (!tree || !tree->left)
		return 1;
	left_result = execute_tree(tree->left, env, exit_status);
	if (left_result == 0 && tree->right)
		return execute_tree(tree->right, env, left_result);
	return left_result;
}

int execute_logical_or(t_tree *tree, t_env **env, int exit_status)
{
	int left_result;
	
	if (!tree || !tree->left)
		return 1;
	left_result = execute_tree(tree->left, env, exit_status);
	if (left_result != 0 && tree->right)
		return execute_tree(tree->right, env, left_result);
	return left_result;
}

// ========== PARENTHESES EXECUTION ==========

int execute_parentheses(t_tree *tree, t_env **env, int exit_status)
{
	int saved_stdin;
	int saved_stdout;
	int result;
	
	if (!tree || !tree->right)
		return 1;
	if (tree->tokens)
	{
		if (setup_redirections(tree->tokens, &saved_stdin, 
							  &saved_stdout, *env, exit_status) == -1)
			return 1;
	}
	else
	{
		saved_stdin = dup(STDIN_FILENO);
		saved_stdout = dup(STDOUT_FILENO);
	}
	result = execute_tree(tree->right, env, exit_status);
	restore_redirections(saved_stdin, saved_stdout);
	return result;
}

// ========== MAIN EXECUTION FUNCTION ==========

int execute_tree(t_tree *tree, t_env **env, int exit_status)
{
	if (!tree || !env)
		return 1;
	if (tree->type == NODE_COMMAND)
		return execute_simple_command(tree->tokens, env, exit_status);
	if (tree->type == NODE_PIPE)
		return execute_pipeline(tree, env, exit_status);
	if (tree->type == NODE_AND)
		return execute_logical_and(tree, env, exit_status);
	if (tree->type == NODE_OR)
		return execute_logical_or(tree, env, exit_status);
	if (tree->type == NODE_PAREN)
		return execute_parentheses(tree, env, exit_status);
	ft_printf("Unknown node type: %d\n", tree->type);
	return 1;
}