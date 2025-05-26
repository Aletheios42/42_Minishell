/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_v4.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elorente <elorente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 16:23:23 by elorente          #+#    #+#             */
/*   Updated: 2025/05/26 16:23:23 by elorente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Inc/minishell.h"
#include "../Inc/signals.h"
#include "../libft/libft.h"

int	fork_and_execute(char *cmd_path, char **args, char **envp)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == 0)
	{
		setup_child_signals();
		execve(cmd_path, args, envp);
		perror("execve");
		exit(127);
	}
	if (pid > 0)
	{
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
		waitpid(pid, &status, 0);
		setup_signals();
		return (WEXITSTATUS(status));
	}
	perror("fork");
	return (1);
}

int	execute_external_command(char **args, t_env *env)
{
	char	*cmd_path;
	char	**envp;
	int		result;

	if (!args || !args[0])
		return (1);
	cmd_path = find_command_in_path(args[0], env);
	if (!cmd_path)
	{
		ft_printf("minishell: %s: command not found\n", args[0]);
		return (127);
	}
	envp = env_to_array(env);
	if (!envp)
	{
		free(cmd_path);
		return (1);
	}
	result = fork_and_execute(cmd_path, args, envp);
	free(cmd_path);
	ft_free_matrix(envp);
	return (result);
}

// ========== REDIRECTION HANDLING ==========

int	create_heredoc_temp_file(void)
{
	char	filename[64];
	char	*number;
	int		fd;
	int		i;

	i = 0;
	while (i < 10000)
	{
		number = ft_itoa(i);
		if (!number)
			return (-1);
		ft_strcpy(filename, "/tmp/.minishell_heredoc_");
		ft_strcat(filename, number);
		free(number);
		fd = open(filename, O_CREAT | O_EXCL | O_RDWR, 0600);
		if (fd != -1)
			return (fd);
		i++;
	}
	return (-1);
}

int	ft_mkstemp(char *buffer, size_t size)
{
	char	*number;
	int		fd;
	int		i;

	if (size < 32)
		return (-1);
	i = 0;
	while (i < 10000)
	{
		number = ft_itoa(i);
		if (!number)
			return (-1);
		ft_strcpy(buffer, "/tmp/.minishell_heredoc_");
		ft_strcat(buffer, number);
		fd = open(buffer, O_CREAT | O_EXCL | O_RDWR, 0600);
		if (fd != -1)
			return (fd);
		i++;
	}
	return (-1);
}

int	write_heredoc_content(int temp_fd, char *delimiter, t_env *env, int exit)
{
	char	*line;
	char	*expanded_line;

	while (1)
	{
		ft_putstr_fd("heredoc> ", STDOUT_FILENO);
		line = get_next_line(STDIN_FILENO);
		if (!line)
			break ;
		if (ft_strncmp(delimiter, line, ft_strlen(delimiter)) == 0
			&& (line[ft_strlen(delimiter)] == '\n'
				|| line[ft_strlen(delimiter)] == '\0'))
		{
			free(line);
			break ;
		}
		expanded_line = expand_string(line, env, exit);
		write(temp_fd, expanded_line, ft_strlen(expanded_line));
		free(line);
		free(expanded_line);
	}
	return (0);
}
