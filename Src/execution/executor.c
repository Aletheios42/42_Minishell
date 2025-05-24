#include "../../Inc/minishell.h"
#include "../../Inc/signals.h"
#include "../../libft/libft.h"

// ========== COMMAND TYPE DETECTION ==========

typedef enum e_command_type {
    CMD_ASSIGNMENT,
    CMD_BUILTIN,
    CMD_EXTERNAL,
    CMD_INVALID
} t_command_type;

int is_builtin_name(const char *cmd)
{
    if (ft_strcmp(cmd, "echo") == 0)
        return 1;
    if (ft_strcmp(cmd, "cd") == 0)
        return 1;
    if (ft_strcmp(cmd, "pwd") == 0)
        return 1;
    if (ft_strcmp(cmd, "export") == 0)
        return 1;
    if (ft_strcmp(cmd, "unset") == 0)
        return 1;
    if (ft_strcmp(cmd, "env") == 0)
        return 1;
    if (ft_strcmp(cmd, "exit") == 0)
        return 1;
    return 0;
}

char *get_first_command_word(t_token *tokens)
{
    t_token *current;
    t_token *prev;
    
    current = tokens;
    prev = NULL;
    while (current)
    {
        if (current->type == TOKEN_WORD || current->type == TOKEN_LITERAL_WORD)
        {
            if (!prev || (prev->type != TOKEN_REDIR_IN && 
                         prev->type != TOKEN_REDIR_OUT && 
                         prev->type != TOKEN_APPEND && 
                         prev->type != TOKEN_HEREDOC))
                return current->value;
        }
        prev = current;
        current = current->next;
    }
    return NULL;
}

t_command_type classify_command(t_token *tokens, t_env *env)
{
    char *first_word;
    char *cmd_path;
    
    if (!tokens)
        return CMD_INVALID;
    first_word = get_first_command_word(tokens);
    if (!first_word)
        return CMD_INVALID;
    if (is_assignment(first_word))
        return CMD_ASSIGNMENT;
    if (is_builtin_name(first_word))
        return CMD_BUILTIN;
    cmd_path = find_command_in_path(first_word, env);
    if (cmd_path)
    {
        free(cmd_path);
        return CMD_EXTERNAL;
    }
    return CMD_INVALID;
}

// ========== BUILTIN EXECUTION ==========

int execute_echo_builtin(char **args)
{
    return ft_echo(args);
}

int execute_cd_builtin(char **args, t_env **env)
{
    return ft_cd(args, env);
}

int execute_export_builtin(char **args, t_env **env)
{
    return ft_export(args, *env, *env);
}

int execute_builtin_command(char **args, t_env **env)
{
    int result;
    
    if (!args || !args[0])
        return 1;
    if (ft_strcmp(args[0], "echo") == 0)
        return execute_echo_builtin(args);
    if (ft_strcmp(args[0], "cd") == 0)
        return execute_cd_builtin(args, env);
    if (ft_strcmp(args[0], "pwd") == 0)
        return ft_pwd(*env);
    if (ft_strcmp(args[0], "export") == 0)
        return execute_export_builtin(args, env);
    if (ft_strcmp(args[0], "unset") == 0)
        return ft_unset(args, env);
    if (ft_strcmp(args[0], "env") == 0)
        return ft_env(*env);
    if (ft_strcmp(args[0], "exit") == 0)
    {
        result = ft_exit(args);
        if (result == 0)
            return 1000;
        if (result == 1)
            return 1001;
        if (result == 255)
            return 1255;
        return 1000 + (result % 256);
    }
    return 1;
}

// ========== TOKEN TO ARGS CONVERSION ==========

int should_include_token(t_token *token, t_token *prev)
{
    if (token->type != TOKEN_WORD && token->type != TOKEN_LITERAL_WORD)
        return 0;
    if (!prev)
        return 1;
    if (prev->type == TOKEN_REDIR_IN || prev->type == TOKEN_REDIR_OUT || 
        prev->type == TOKEN_APPEND || prev->type == TOKEN_HEREDOC)
        return 0;
    return 1;
}

int count_word_tokens(t_token *tokens)
{
    t_token *current;
    t_token *prev;
    int count;
    
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
    return count;
}

int fill_args_array(char **args, t_token *tokens)
{
    t_token *current;
    t_token *prev;
    int i;
    
    i = 0;
    current = tokens;
    prev = NULL;
    while (current)
    {
        if (should_include_token(current, prev))
        {
            args[i] = ft_strdup(current->value);
            if (!args[i])
                return 0;
            i++;
        }
        prev = current;
        current = current->next;
    }
    args[i] = NULL;
    return 1;
}

char **tokens_to_args_array(t_token *tokens)
{
    char **args;
    int count;
    
    count = count_word_tokens(tokens);
    if (count == 0)
        return NULL;
    args = malloc(sizeof(char *) * (count + 1));
    if (!args)
        return NULL;
    if (!fill_args_array(args, tokens))
    {
        ft_free_matrix(args);
        return NULL;
    }
    return args;
}

// ========== EXTERNAL COMMAND EXECUTION ==========

char *check_absolute_path(const char *cmd)
{
    if (ft_strchr(cmd, '/'))
    {
        if (access(cmd, X_OK) == 0)
            return ft_strdup(cmd);
    }
    return NULL;
}

char *search_in_path_dirs(const char *cmd, char **path_dirs)
{
    char *temp;
    char *full_path;
    int i;
    
    i = 0;
    while (path_dirs[i])
    {
        temp = ft_strjoin(path_dirs[i], "/");
        if (temp)
        {
            full_path = ft_strjoin(temp, cmd);
            free(temp);
            if (full_path && access(full_path, X_OK) == 0)
                return full_path;
            free(full_path);
        }
        i++;
    }
    return NULL;
}

char *find_command_in_path(const char *cmd, t_env *env)
{
    char *path_env;
    char **path_dirs;
    char *result;
    
    if (!cmd)
        return NULL;
    result = check_absolute_path(cmd);
    if (result)
        return result;
    path_env = get_env_value(env, "PATH");
    if (!path_env)
        return NULL;
    path_dirs = ft_split(path_env, ':');
    if (!path_dirs)
        return NULL;
    result = search_in_path_dirs(cmd, path_dirs);
    ft_free_matrix(path_dirs);
    return result;
}

int fork_and_execute(char *cmd_path, char **args, char **envp)
{
    pid_t pid;
    int status;
    
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
        return WEXITSTATUS(status);
    }
    perror("fork");
    return 1;
}

int execute_external_command(char **args, t_env *env)
{
    char *cmd_path;
    char **envp;
    int result;
    
    if (!args || !args[0])
        return 1;
    cmd_path = find_command_in_path(args[0], env);
    if (!cmd_path)
    {
        ft_printf("minishell: %s: command not found\n", args[0]);
        return 127;
    }
    envp = env_to_array(env);
    if (!envp)
    {
        free(cmd_path);
        return 1;
    }
    result = fork_and_execute(cmd_path, args, envp);
    free(cmd_path);
    ft_free_matrix(envp);
    return result;
}

// ========== REDIRECTION HANDLING ==========

int create_heredoc_temp_file(void)
{
    char temp_filename[] = "/tmp/.minishell_heredoc_XXXXXX";
    
    return mkstemp(temp_filename);
}

int write_heredoc_content(int temp_fd, char *delimiter, t_env *env, int exit_status)
{
    char *line;
    char *expanded_line;
    
    while (1)
    {
        ft_putstr_fd("heredoc> ", STDOUT_FILENO);
        line = get_next_line(STDIN_FILENO);
        if (!line)
            break;
        if (ft_strncmp(delimiter, line, ft_strlen(delimiter)) == 0 && 
            (line[ft_strlen(delimiter)] == '\n' || 
             line[ft_strlen(delimiter)] == '\0'))
        {
            free(line);
            break;
        }
        expanded_line = expand_string(line, env, exit_status);
        write(temp_fd, expanded_line, ft_strlen(expanded_line));
        free(line);
        free(expanded_line);
    }
    return 0;
}

int handle_heredoc(t_token *redir_token, t_env *env, int exit_status)
{
    char *delimiter;
    int temp_fd;
    char temp_filename[] = "/tmp/.minishell_heredoc_XXXXXX";
    
    if (!redir_token || !redir_token->next || !redir_token->next->value)
        return -1;
    delimiter = redir_token->next->value;
    temp_fd = mkstemp(temp_filename);
    if (temp_fd == -1)
    {
        perror("mkstemp");
        return -1;
    }
    write_heredoc_content(temp_fd, delimiter, env, exit_status);
    close(temp_fd);
    temp_fd = open(temp_filename, O_RDONLY);
    if (temp_fd == -1)
    {
        perror("open");
        unlink(temp_filename);
        return -1;
    }
    unlink(temp_filename);
    return temp_fd;
}

int setup_input_redirection(t_token *redir_token, t_env *env, int exit_status)
{
    int fd;
    
    if (redir_token->type == TOKEN_HEREDOC)
    {
        fd = handle_heredoc(redir_token, env, exit_status);
        if (fd == -1)
            return -1;
        if (dup2(fd, STDIN_FILENO) == -1)
        {
            perror("dup2");
            close(fd);
            return -1;
        }
        close(fd);
    }
    else if (redir_token->type == TOKEN_REDIR_IN)
    {
        fd = open(redir_token->next->value, O_RDONLY);
        if (fd == -1)
        {
            perror(redir_token->next->value);
            return -1;
        }
        if (dup2(fd, STDIN_FILENO) == -1)
        {
            perror("dup2");
            close(fd);
            return -1;
        }
        close(fd);
    }
    return 0;
}

int setup_output_redirection(t_token *redir_token)
{
    int fd;
    int flags;
    
    if (redir_token->type == TOKEN_APPEND)
        flags = O_WRONLY | O_CREAT | O_APPEND;
    else if (redir_token->type == TOKEN_REDIR_OUT)
        flags = O_WRONLY | O_CREAT | O_TRUNC;
    else
        return -1;
    fd = open(redir_token->next->value, flags, 0644);
    if (fd == -1)
    {
        perror(redir_token->next->value);
        return -1;
    }
    if (dup2(fd, STDOUT_FILENO) == -1)
    {
        perror("dup2");
        close(fd);
        return -1;
    }
    close(fd);
    return 0;
}

int setup_redirections(t_token *tokens, int *saved_stdin, 
                      int *saved_stdout, t_env *env, int exit_status)
{
    t_token *current;
    
    *saved_stdin = dup(STDIN_FILENO);
    *saved_stdout = dup(STDOUT_FILENO);
    if (*saved_stdin == -1 || *saved_stdout == -1)
    {
        perror("dup");
        return -1;
    }
    current = tokens;
    while (current)
    {
        if (current->type == TOKEN_REDIR_IN || current->type == TOKEN_HEREDOC)
        {
            if (setup_input_redirection(current, env, exit_status) == -1)
                return -1;
        }
        else if (current->type == TOKEN_REDIR_OUT || 
                 current->type == TOKEN_APPEND)
        {
            if (setup_output_redirection(current) == -1)
                return -1;
        }
        current = current->next;
    }
    return 0;
}

void restore_redirections(int saved_stdin, int saved_stdout)
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

// ========== ASSIGNMENT EXECUTION ==========

int execute_assignments(t_token *tokens, t_env **env)
{
    t_token *current;
    int assignment_count;
    char *key;
    char *value;
    char *expanded_value;
    
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
    return assignment_count;
}

// ========== COMMAND EXECUTION ==========

int execute_simple_command(t_token *tokens, t_env **env, int exit_status)
{
    t_command_type cmd_type;
    t_token *expanded_tokens;
    char **args;
    int result;
    int saved_stdin;
    int saved_stdout;
    
    if (!tokens)
        return 0;
    execute_assignments(tokens, env);
    expanded_tokens = expand_token_list_copy(tokens, *env, exit_status);
    if (!expanded_tokens)
        return 1;
    cmd_type = classify_command(expanded_tokens, *env);
    if (cmd_type == CMD_ASSIGNMENT)
    {
        free_token_list(expanded_tokens);
        return 0;
    }
    if (setup_redirections(expanded_tokens, &saved_stdin, 
                          &saved_stdout, *env, exit_status) == -1)
    {
        free_token_list(expanded_tokens);
        return 1;
    }
    args = tokens_to_args_array(expanded_tokens);
    if (!args)
    {
        restore_redirections(saved_stdin, saved_stdout);
        free_token_list(expanded_tokens);
        return 0;
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
    return result;
}

// ========== PIPELINE EXECUTION ==========

int setup_pipeline_fds(int pipefd[2])
{
    if (pipe(pipefd) == -1)
    {
        perror("pipe");
        return -1;
    }
    return 0;
}

int fork_left_process(t_tree *tree, t_env **env, int exit_status, int pipefd[2])
{
    pid_t left_pid;
    
    left_pid = fork();
    if (left_pid == 0)
    {
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);
        exit(execute_tree(tree->left, env, exit_status));
    }
    return left_pid;
}

int fork_right_process(t_tree *tree, t_env **env, int exit_status, int pipefd[2])
{
    pid_t right_pid;
    
    right_pid = fork();
    if (right_pid == 0)
    {
        close(pipefd[1]);
        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[0]);
        exit(execute_tree(tree->right, env, exit_status));
    }
    return right_pid;
}

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
