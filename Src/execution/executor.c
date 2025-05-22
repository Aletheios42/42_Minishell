/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alepinto <alepinto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 15:00:00 by alepinto          #+#    #+#             */
/*   Updated: 2025/05/23 15:00:00 by alepinto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Inc/minishell.h"
#include "../../libft/libft.h"

static int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	return (ft_strcmp(cmd, "echo") == 0 || ft_strcmp(cmd, "cd") == 0 ||
			ft_strcmp(cmd, "pwd") == 0 || ft_strcmp(cmd, "export") == 0 ||
			ft_strcmp(cmd, "unset") == 0 || ft_strcmp(cmd, "env") == 0 ||
			ft_strcmp(cmd, "exit") == 0);
}

static int	execute_builtin(char **args, t_env **env, t_local_vars **locals)
{
	if (ft_strcmp(args[0], "echo") == 0)
		return (ft_echo(args));
	if (ft_strcmp(args[0], "cd") == 0)
		return (ft_cd(args, env));
	if (ft_strcmp(args[0], "pwd") == 0)
		return (ft_pwd());
	if (ft_strcmp(args[0], "export") == 0)
		return (ft_export(args, *env, *env));
	if (ft_strcmp(args[0], "unset") == 0)
		return (ft_unset(args, env, locals));
	if (ft_strcmp(args[0], "env") == 0)
		return (ft_env(*env));
	if (ft_strcmp(args[0], "exit") == 0)
		return (ft_exit(args));
	return (1);
}

static int	is_assignment(char *str)
{
	int	i;

	i = 0;
	if (!str || (!ft_isalpha(str[0]) && str[0] != '_'))
		return (0);
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (str[i] == '=');
}

static int	process_assignment(char *assignment, t_local_vars **locals)
{
	char	*equal_pos;
	char	*key;
	char	*value;

	equal_pos = ft_strchr(assignment, '=');
	if (!equal_pos)
		return (1);
	key = ft_substr(assignment, 0, equal_pos - assignment);
	if (!key)
		return (1);
	value = ft_strdup(equal_pos + 1);
	if (!value)
	{
		free(key);
		return (1);
	}
	local_var_set(locals, key, value);
	free(key);
	free(value);
	return (0);
}

static int	count_tokens(t_token *tokens)
{
	int		count;
	t_token	*temp;

	temp = tokens;
	count = 0;
	while (temp)
	{
		count++;
		temp = temp->next;
	}
	return (count);
}

static char	**tokens_to_args(t_token *tokens)
{
	char	**args;
	int		count;
	int		i;

	count = count_tokens(tokens);
	args = malloc(sizeof(char *) * (count + 1));
	if (!args)
		return (NULL);
	i = 0;
	while (tokens)
	{
		args[i] = ft_strdup(tokens->value);
		if (!args[i])
		{
			ft_free_matrix(args);
			return (NULL);
		}
		tokens = tokens->next;
		i++;
	}
	args[i] = NULL;
	return (args);
}

static int	count_env(t_env *env)
{
	int		count;
	t_env	*temp;

	temp = env;
	count = 0;
	while (temp)
	{
		count++;
		temp = temp->next;
	}
	return (count);
}

static char	**env_to_array(t_env *env)
{
	char	**envp;
	int		count;
	int		i;

	count = count_env(env);
	envp = malloc(sizeof(char *) * (count + 1));
	if (!envp)
		return (NULL);
	i = 0;
	while (env)
	{
		envp[i] = ft_strdup(env->value);
		if (!envp[i])
		{
			ft_free_matrix(envp);
			return (NULL);
		}
		env = env->next;
		i++;
	}
	envp[i] = NULL;
	return (envp);
}

static int	handle_heredoc_loop(int temp_fd, char *delimiter)
{
	char	*line;

	while (1)
	{
		ft_putstr_fd("heredoc> ", STDOUT_FILENO);
		line = get_next_line(STDIN_FILENO);
		if (!line)
			break ;
		if (ft_strncmp(delimiter, line, ft_strlen(delimiter)) == 0 &&
			line[ft_strlen(delimiter)] == '\n')
		{
			free(line);
			break ;
		}
		write(temp_fd, line, ft_strlen(line));
		free(line);
	}
	return (0);
}

static int	handle_heredoc(t_token *token)
{
	char	*delimiter;
	int		temp_fd;

	if (!token || !token->next || !token->next->value)
		return (-1);
	delimiter = token->next->value;
	temp_fd = open("/tmp/.minishell_heredoc", O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (temp_fd == -1)
		return (-1);
	handle_heredoc_loop(temp_fd, delimiter);
	close(temp_fd);
	return (open("/tmp/.minishell_heredoc", O_RDONLY));
}

static char	*build_full_path(char *dir, char *cmd)
{
	char	*temp;
	char	*full_path;

	temp = ft_strjoin(dir, "/");
	full_path = ft_strjoin(temp, cmd);
	free(temp);
	return (full_path);
}

static char	*search_in_paths(char **paths, char *cmd)
{
	char	*full_path;
	int		i;

	i = 0;
	while (paths[i])
	{
		full_path = build_full_path(paths[i], cmd);
		if (access(full_path, X_OK) == 0)
		{
			ft_free_matrix(paths);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	ft_free_matrix(paths);
	return (NULL);
}

static char	*find_command_path(char *cmd, t_env *env)
{
	char	*path_env;
	char	**paths;

	if (ft_strchr(cmd, '/'))
		return (ft_strdup(cmd));
	path_env = get_env_value(env, "PATH");
	if (!path_env)
		return (NULL);
	paths = ft_split(path_env, ':');
	if (!paths)
		return (NULL);
	return (search_in_paths(paths, cmd));
}

static int	fork_and_exec(char *cmd_path, char **args, char **envp)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == 0)
	{
		execve(cmd_path, args, envp);
		perror("execve");
		exit(127);
	}
	else if (pid > 0)
	{
		waitpid(pid, &status, 0);
		return (WEXITSTATUS(status));
	}
	return (1);
}

static int	execute_external_command(char **args, t_env *env)
{
	char	**envp;
	char	*cmd_path;
	int		status;

	cmd_path = find_command_path(args[0], env);
	if (!cmd_path)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(args[0], 2);
		ft_putendl_fd(": command not found", 2);
		return (127);
	}
	envp = env_to_array(env);
	if (!envp)
	{
		free(cmd_path);
		return (1);
	}
	status = fork_and_exec(cmd_path, args, envp);
	free(cmd_path);
	ft_free_matrix(envp);
	return (status);
}

static void	handle_heredocs(t_token *tokens)
{
	t_token	*current;
	int		heredoc_fd;

	current = tokens;
	while (current)
	{
		if (current->type == TOKEN_HEREDOC)
		{
			heredoc_fd = handle_heredoc(current);
			if (heredoc_fd != -1)
			{
				dup2(heredoc_fd, STDIN_FILENO);
				close(heredoc_fd);
			}
		}
		current = current->next;
	}
}

static void	expand_tokens_safe(t_token *tokens, t_env *env, 
								t_local_vars **locals, int exit_status)
{
	if (locals)
		expand_token_list(tokens, env, *locals, exit_status);
	else
		expand_token_list(tokens, env, NULL, exit_status);
}

// ========== 1. NUEVAS FUNCIONES AUXILIARES ==========
// Agregar a Src/execution/executor.c

static char	*join_tokens_to_string(t_token *tokens)
{
	char	*result;
	char	*temp;
	t_token	*current;

	if (!tokens)
		return (ft_strdup(""));
	result = ft_strdup("");
	current = tokens;
	while (current)
	{
		temp = ft_strjoin(result, current->value);
		free(result);
		result = temp;
		if (current->next)
		{
			temp = ft_strjoin(result, " ");
			free(result);
			result = temp;
		}
		current = current->next;
	}
	return (result);
}

static t_token	*retokenize_string(char *str)
{
	char	**words;
	t_token	*head;
	t_token	*current;
	t_token	*new_token;
	int		i;

	if (!str || !*str)
		return (NULL);
	words = ft_split(str, ' ');
	if (!words)
		return (NULL);
	head = NULL;
	current = NULL;
	i = 0;
	while (words[i])
	{
		new_token = create_token(ft_strdup(words[i]), TOKEN_WORD);
		if (!new_token)
		{
			ft_free_matrix(words);
			free_token_list(head);
			return (NULL);
		}
		if (!head)
			head = current = new_token;
		else
		{
			current->next = new_token;
			new_token->prev = current;
			current = new_token;
		}
		i++;
	}
	ft_free_matrix(words);
	return (head);
}

static int	tokens_need_retokenization(t_token *tokens)
{
	t_token	*current;

	current = tokens;
	while (current)
	{
		if (ft_strchr(current->value, ' ') || ft_strchr(current->value, '\t'))
			return (1);
		current = current->next;
	}
	return (0);
}

// ========== 2. FUNCIÓN EXECUTE_COMMAND MODIFICADA ==========
// Reemplazar la función execute_command existente

static int	execute_command(t_token *tokens, t_env **env, 
							t_local_vars **locals, int exit_status)
{
	char	**args;
	int		status;
	char	*expanded_string;
	t_token	*retokenized;

	if (!tokens)
		return (0);
	handle_heredocs(tokens);
	printf("DEBUG: Original tokens: ");
	ft_print_token_list(tokens);
	printf("\n");
	
	// 1. Expandir variables en tokens originales
	expand_tokens_safe(tokens, *env, locals, exit_status);
	printf("DEBUG: After expansion: ");
	ft_print_token_list(tokens);
	printf("\n");
	
	// 2. Re-tokenizar si hay espacios en valores expandidos
	if (tokens_need_retokenization(tokens))
	{
		printf("DEBUG: Re-tokenization needed\n");
		expanded_string = join_tokens_to_string(tokens);
		printf("DEBUG: Joined string: '%s'\n", expanded_string);
		retokenized = retokenize_string(expanded_string);
		free(expanded_string);
		
		if (retokenized)
		{
			printf("DEBUG: After retokenization: ");
			ft_print_token_list(retokenized);
			printf("\n");
			// Usar tokens re-tokenizados
			free_token_list(tokens);
			tokens = retokenized;
		}
	}
	
	// 3. Procesar asignaciones
	if (is_assignment(tokens->value))
		return (process_assignment(tokens->value, locals));
	
	// 4. Ejecutar comando
	args = tokens_to_args(tokens);
	if (!args || !args[0])
		return (1);
	if (is_builtin(args[0]))
		status = execute_builtin(args, env, locals);
	else
		status = execute_external_command(args, *env);
	ft_free_matrix(args);
	return (status);
}

static int	execute_pipe(t_tree *tree, t_env **env, 
						t_local_vars **locals, int exit_status)
{
	int		pipefd[2];
	pid_t	pid1;
	pid_t	pid2;
	int		status;

	if (pipe(pipefd) == -1)
		return (1);
	pid1 = fork();
	if (pid1 == 0)
	{
		close(pipefd[0]);
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);
		exit(execute_tree(tree->left, env, locals, exit_status));
	}
	pid2 = fork();
	if (pid2 == 0)
	{
		close(pipefd[1]);
		dup2(pipefd[0], STDIN_FILENO);
		close(pipefd[0]);
		exit(execute_tree(tree->right, env, locals, exit_status));
	}
	close(pipefd[0]);
	close(pipefd[1]);
	waitpid(pid1, NULL, 0);
	waitpid(pid2, &status, 0);
	return (WEXITSTATUS(status));
}

int execute_tree(t_tree *tree, t_env **env, t_local_vars **locals, int exit_status)
{
    int left_status;

    if (!tree)
        return (0);
    
    printf("DEBUG execute_tree: type=%d, locals=%p\n", tree->type, locals ? *locals : NULL);
    
    if (tree->type == NODE_COMMAND)
        return (execute_command(tree->tokens, env, locals, exit_status));
    if (tree->type == NODE_PIPE)
        return (execute_pipe(tree, env, locals, exit_status));
    if (tree->type == NODE_AND)
    {
        printf("DEBUG: AND - executing left node\n");
        left_status = execute_tree(tree->left, env, locals, exit_status);
        printf("DEBUG: AND - left done, locals=%p, executing right\n", locals ? *locals : NULL);
        if (left_status == 0)
            return (execute_tree(tree->right, env, locals, left_status));
        return (left_status);
    }
    // ... resto igual
	if (tree->type == NODE_OR)
	{
		// Mismo principio para OR
		left_status = execute_tree(tree->left, env, locals, exit_status);
		if (left_status != 0)
			return (execute_tree(tree->right, env, locals, left_status));
		return (left_status);
	}
	return (1);
}
