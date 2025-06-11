/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_v7.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elorente <elorente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 00:24:10 by alepinto          #+#    #+#             */
/*   Updated: 2025/06/11 17:09:15 by elorente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Inc/minishell.h"
#include "../Inc/signals.h"
#include "../libft/libft.h"

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

int	close_fds_restore(int in, int out)
{
	dup2(in, STDIN_FILENO);
	dup2(out, STDOUT_FILENO);
	close(in);
	close(out);
	return (0);
}

int	handle_fork_error(t_pipeline_ctx *ctx, int kill_left)
{
	perror("fork");
	close(ctx->pipefd[0]);
	close(ctx->pipefd[1]);
	if (kill_left)
	{
		kill(ctx->left_pid, SIGTERM);
		waitpid(ctx->left_pid, NULL, 0);
	}
	return (close_fds_restore(ctx->saved_stdin, ctx->saved_stdout), 1);
}

int	finalize_pipeline(t_pipeline_ctx *ctx)
{
	close(ctx->pipefd[0]);
	close(ctx->pipefd[1]);
	waitpid(ctx->left_pid, NULL, 0);
	waitpid(ctx->right_pid, &ctx->status, 0);
	close_fds_restore(ctx->saved_stdin, ctx->saved_stdout);
	return (WEXITSTATUS(ctx->status));
}
