/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_v6.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elorente <elorente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 19:21:24 by elorente          #+#    #+#             */
/*   Updated: 2025/06/09 19:59:09 by elorente         ###   ########.fr       */
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

int	prepare_arguments(t_exec_context *ctx)
{
	ctx->args = tokens_to_args_array(ctx->expanded_tokens);
	if (!ctx->args || !ctx->args[0])
	{
		restore_redirections(ctx->redir_ctx.saved_stdin,
			ctx->redir_ctx.saved_stdout);
		free_token_list(ctx->expanded_tokens);
		if (ctx->args)
			ft_free_matrix(ctx->args);
		return (0);
	}
	return (1);
}

void	execute_final_command(t_exec_context *ctx, t_env **env)
{
	if (!ctx->args || !ctx->args[0] || ctx->args[0][0] == '\0')
	{
		ft_putendl_fd("minishell: command not found", 2);
		ctx->result = 127;
		return ;
	}
	if (ctx->cmd_type == CMD_BUILTIN)
		ctx->result = execute_builtin_command(ctx->args, env);
	else if (ctx->cmd_type == CMD_EXTERNAL)
		ctx->result = execute_external_command(ctx->args, *env);
	else
	{
		ft_printf("minishell: %s: command not found\n", ctx->args[0]);
		ctx->result = 127;
	}
}

// ========== COMMAND EXECUTION ==========

int	execute_simple_command(t_token *tokens, t_env **env, int exit_status)
{
	t_exec_context	ctx;

	if (!tokens)
		return (0);
	execute_assignments(tokens, env);
	ctx.expanded_tokens = expand_token_list_copy(tokens, *env, exit_status);
	if (!ctx.expanded_tokens)
		return (1);
	ctx.cmd_type = classify_command(ctx.expanded_tokens, *env);
	if (ctx.cmd_type == CMD_ASSIGNMENT)
		return (free_token_list(ctx.expanded_tokens), 0);
	if (setup_redirections(ctx.expanded_tokens, *env,
			exit_status, &ctx.redir_ctx) == -1)
		return (free_token_list(ctx.expanded_tokens), 1);
	if (!prepare_arguments(&ctx))
		return (0);
	execute_final_command(&ctx, env);
	restore_redirections(ctx.redir_ctx.saved_stdin, ctx.redir_ctx.saved_stdout);
	ft_free_matrix(ctx.args);
	free_token_list(ctx.expanded_tokens);
	return (ctx.result);
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
