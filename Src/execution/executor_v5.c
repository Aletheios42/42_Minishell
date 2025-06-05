/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_v5.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elorente <elorente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 16:31:30 by alepinto          #+#    #+#             */
/*   Updated: 2025/05/27 18:06:42 by elorente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Inc/minishell.h"
#include "../Inc/signals.h"
#include "../libft/libft.h"

int	handle_heredoc(t_token *redir_token, t_env *env, int exit_status)
{
	char	filename[64];
	char	*delimiter;
	int		temp_fd;

	if (!redir_token || !redir_token->next || !redir_token->next->value)
		return (-1);
	delimiter = redir_token->next->value;
	temp_fd = ft_mkstemp(filename, sizeof(filename));
	if (temp_fd == -1)
	{
		perror("ft_mkstemp");
		return (-1);
	}
	write_heredoc_content(temp_fd, delimiter, env, exit_status);
	close(temp_fd);
	temp_fd = open(filename, O_RDONLY);
	if (temp_fd == -1)
	{
		perror("open");
		unlink(filename);
		return (-1);
	}
	unlink(filename);
	return (temp_fd);
}

int	setup_input_redirection(t_token *redir_token, t_env *env, int exit_status)
{
	int	fd;

	if (redir_token->type == TOKEN_HEREDOC)
	{
		fd = handle_heredoc(redir_token, env, exit_status);
		if (fd == -1)
			return (-1);
		if (dup2(fd, STDIN_FILENO) == -1)
		{
			perror("dup2");
			close(fd);
			return (-1);
		}
		close(fd);
	}
	else if (redir_token->type == TOKEN_REDIR_IN)
	{
		fd = open(redir_token->next->value, O_RDONLY);
		if (fd == -1)
			return (perror(redir_token->next->value), -1);
		if (dup2(fd, STDIN_FILENO) == -1)
			return (perror("dup2"), close(fd), -1);
		close(fd);
	}
	return (0);
}

int	setup_output_redirection(t_token *redir_token)
{
	int	fd;
	int	flags;

	if (redir_token->type == TOKEN_APPEND)
		flags = O_WRONLY | O_CREAT | O_APPEND;
	else if (redir_token->type == TOKEN_REDIR_OUT)
		flags = O_WRONLY | O_CREAT | O_TRUNC;
	else
		return (-1);
	fd = open(redir_token->next->value, flags, 0644);
	if (fd == -1)
	{
		perror(redir_token->next->value);
		return (-1);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		perror("dup2");
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

int	setup_redirections(t_token *tokens, t_env *env,
	int exit_status, t_redir_ctx *ctx)
{
	t_token	*current;

	ctx->saved_stdin = dup(STDIN_FILENO);
	ctx->saved_stdout = dup(STDOUT_FILENO);
	if (ctx->saved_stdin == -1 || ctx->saved_stdout == -1)
		return (perror("dup"), -1);
	current = tokens;
	while (current)
	{
		if (current->type == TOKEN_REDIR_IN || current->type == TOKEN_HEREDOC)
		{
			if (setup_input_redirection(current, env, exit_status) == -1)
				return (-1);
		}
		else if (current->type == TOKEN_REDIR_OUT
			|| current->type == TOKEN_APPEND)
		{
			if (setup_output_redirection(current) == -1)
				return (-1);
		}
		current = current->next;
	}
	return (0);
}

void	restore_redirections(int saved_stdin, int saved_stdout)
{
	if (saved_stdin != -1)
	{
		dup2(saved_stdin, STDIN_FILENO);
		close(saved_stdin);
	}
	if (saved_stdout != -1)
	{
		dup2(saved_stdout, STDOUT_FILENO);
		close(saved_stdout);
	}
}
