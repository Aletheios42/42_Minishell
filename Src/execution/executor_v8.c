/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_v8.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elorente <elorente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 11:58:06 by elorente          #+#    #+#             */
/*   Updated: 2025/06/11 20:06:16 by elorente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Inc/minishell.h"
#include "../Inc/signals.h"
#include "../libft/libft.h"

int	execute_pipeline(t_tree *tree, t_env **env, int exit_status)
{
	t_pipeline_ctx	ctx;

	if (!tree || !tree->left || !tree->right)
		return (1);
	ctx.saved_stdin = dup(STDIN_FILENO);
	ctx.saved_stdout = dup(STDOUT_FILENO);
	if (setup_pipeline_fds(ctx.pipefd) == -1)
		return (close_fds_restore(ctx.saved_stdin, ctx.saved_stdout), 1);
	ctx.left_pid = fork_left_process(tree, env, exit_status, ctx.pipefd);
	if (ctx.left_pid == -1)
		return (handle_fork_error(&ctx, 0));
	ctx.right_pid = fork_right_process(tree, env, exit_status, ctx.pipefd);
	if (ctx.right_pid == -1)
		return (handle_fork_error(&ctx, 1));
	return (finalize_pipeline(&ctx));
}
// ========== LOGICAL OPERATIONS ==========

int	execute_logical_and(t_tree *tree, t_env **env, int exit_status)
{
	int	left_result;

	if (!tree || !tree->left)
		return (1);
	left_result = execute_tree(tree->left, env, exit_status);
	if (left_result == 0 && tree->right)
		return (execute_tree(tree->right, env, left_result));
	return (left_result);
}

int	execute_logical_or(t_tree *tree, t_env **env, int exit_status)
{
	int	left_result;

	if (!tree || !tree->left)
		return (1);
	left_result = execute_tree(tree->left, env, exit_status);
	if (left_result != 0 && tree->right)
		return (execute_tree(tree->right, env, left_result));
	return (left_result);
}

// ========== PARENTHESES EXECUTION ==========

int	execute_parentheses(t_tree *tree, t_env **env, int exit_status)
{
	t_redir_ctx	redir;
	int			result;

	if (!tree || !tree->right)
		return (1);
	if (tree->tokens)
	{
		if (setup_redirections(tree->tokens, *env,
				exit_status, &redir) == -1)
			return (1);
	}
	else
	{
		redir.saved_stdin = dup(STDIN_FILENO);
		redir.saved_stdout = dup(STDOUT_FILENO);
	}
	result = execute_tree(tree->right, env, exit_status);
	restore_redirections(redir.saved_stdin, redir.saved_stdout);
	return (result);
}

// ========== MAIN EXECUTION FUNCTION ==========

int	execute_tree(t_tree *tree, t_env **env, int exit_status)
{
	if (!tree || !env)
		return (1);
	if (tree->type == NODE_COMMAND)
		return (execute_simple_command(tree->tokens, env, exit_status));
	if (tree->type == NODE_PIPE)
		return (execute_pipeline(tree, env, exit_status));
	if (tree->type == NODE_AND)
		return (execute_logical_and(tree, env, exit_status));
	if (tree->type == NODE_OR)
		return (execute_logical_or(tree, env, exit_status));
	if (tree->type == NODE_PAREN)
		return (execute_parentheses(tree, env, exit_status));
	ft_printf("Unknown node type: %d\n", tree->type);
	return (1);
}
