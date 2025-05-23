#include "../../Inc/minishell.h"
#include "../../libft/libft.h"

// ========== COMMAND TYPE DETECTION ==========

typedef enum e_command_type {
    CMD_ASSIGNMENT,
    CMD_BUILTIN,
    CMD_EXTERNAL,
    CMD_INVALID
} t_command_type;

t_command_type classify_command(t_token *tokens, t_env *env)
{
    t_token *current;
    t_token *prev;
    char *first_word;
    
    if (!tokens)
        return CMD_INVALID;
    
    current = tokens;
    prev = NULL;
    first_word = NULL;
    
    // Find first WORD that's not a redirection target
    while (current)
    {
        if (current->type == TOKEN_WORD || current->type == TOKEN_LITERAL_WORD)
        {
            // Skip if previous token was a redirection
            if (!prev || (prev->type != TOKEN_REDIR_IN && 
                         prev->type != TOKEN_REDIR_OUT && 
                         prev->type != TOKEN_APPEND && 
                         prev->type != TOKEN_HEREDOC))
            {
                first_word = current->value;
                break;
            }
        }
        prev = current;
        current = current->next;
    }
    
    if (!first_word)
        return CMD_INVALID;
    
    // Check if it's an assignment
    if (is_assignment(first_word))
        return CMD_ASSIGNMENT;
    
    // Check if it's a builtin
    if (ft_strcmp(first_word, "echo") == 0 ||
        ft_strcmp(first_word, "cd") == 0 ||
        ft_strcmp(first_word, "pwd") == 0 ||
        ft_strcmp(first_word, "export") == 0 ||
        ft_strcmp(first_word, "unset") == 0 ||
        ft_strcmp(first_word, "env") == 0 ||
        ft_strcmp(first_word, "exit") == 0)
        return CMD_BUILTIN;
    
    // Check if it's an external command
    char *cmd_path = find_command_in_path(first_word, env);
    if (cmd_path)
    {
        free(cmd_path);  // ← FIX: liberar el path encontrado
        return CMD_EXTERNAL;
    }
    
    return CMD_INVALID;
}

// ========== BUILTIN EXECUTION ==========

int execute_builtin_command(char **args, t_env **env)
{
    if (!args || !args[0])
        return 1;
    
    if (ft_strcmp(args[0], "echo") == 0)
        return ft_echo(args);
    if (ft_strcmp(args[0], "cd") == 0)
        return ft_cd(args, env);
    if (ft_strcmp(args[0], "pwd") == 0)
        return ft_pwd();
    if (ft_strcmp(args[0], "export") == 0)
        return ft_export(args, *env, *env);
    if (ft_strcmp(args[0], "unset") == 0)
        return ft_unset(args, env);
    if (ft_strcmp(args[0], "env") == 0)
        return ft_env(*env);
    if (ft_strcmp(args[0], "exit") == 0)
        return ft_exit(args);
    
    return 1;
}

// ========== TOKEN TO ARGS CONVERSION ==========

int count_word_tokens(t_token *tokens)
{
    int count;
    
    count = 0;
    while (tokens)
    {
        if (tokens->type == TOKEN_WORD || tokens->type == TOKEN_LITERAL_WORD)
            count++;
        tokens = tokens->next;
    }
    return count;
}

char **tokens_to_args_array(t_token *tokens)
{
    char **args;
    int count;
    int i;
    t_token *current;
    t_token *prev;
    
    count = count_word_tokens(tokens);
    if (count == 0)
        return NULL;
    
    // Count actual args (excluding redirection targets)
    current = tokens;
    prev = NULL;
    count = 0;
    while (current)
    {
        if (current->type == TOKEN_WORD || current->type == TOKEN_LITERAL_WORD)
        {
            // Skip if previous token was a redirection
            if (!prev || (prev->type != TOKEN_REDIR_IN && 
                         prev->type != TOKEN_REDIR_OUT && 
                         prev->type != TOKEN_APPEND && 
                         prev->type != TOKEN_HEREDOC))
                count++;
        }
        prev = current;
        current = current->next;
    }
    
    if (count == 0)
        return NULL;
    
    args = malloc(sizeof(char *) * (count + 1));
    if (!args)
        return NULL;
    
    i = 0;
    current = tokens;
    prev = NULL;
    while (current && i < count)
    {
        if (current->type == TOKEN_WORD || current->type == TOKEN_LITERAL_WORD)
        {
            // Skip if previous token was a redirection
            if (!prev || (prev->type != TOKEN_REDIR_IN && 
                         prev->type != TOKEN_REDIR_OUT && 
                         prev->type != TOKEN_APPEND && 
                         prev->type != TOKEN_HEREDOC))
            {
                args[i] = ft_strdup(current->value);
                if (!args[i])
                {
                    ft_free_matrix(args);
                    return NULL;
                }
                i++;
            }
        }
        prev = current;
        current = current->next;
    }
    
    args[i] = NULL;
    return args;
}

// ========== EXTERNAL COMMAND EXECUTION ==========

char *find_command_in_path(const char *cmd, t_env *env)
{
    char *path_env;
    char **path_dirs;
    char *full_path;
    char *temp;
    int i;
    
    if (!cmd)
        return NULL;
    
    // If command contains '/', use it as-is
    if (ft_strchr(cmd, '/'))
    {
        if (access(cmd, X_OK) == 0)
            return ft_strdup(cmd);
        return NULL;
    }
    
    path_env = get_env_value(env, "PATH");
    if (!path_env)
        return NULL;
    
    path_dirs = ft_split(path_env, ':');
    if (!path_dirs)
        return NULL;
    
    i = 0;
    while (path_dirs[i])
    {
        temp = ft_strjoin(path_dirs[i], "/");
        if (temp)
        {
            full_path = ft_strjoin(temp, cmd);
            free(temp);
            
            if (full_path && access(full_path, X_OK) == 0)
            {
                ft_free_matrix(path_dirs);
                return full_path;
            }
            free(full_path);
        }
        i++;
    }
    
    ft_free_matrix(path_dirs);
    return NULL;
}

int execute_external_command(char **args, t_env *env)
{
    char *cmd_path;
    char **envp;
    pid_t pid;
    int status;
    
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
    
    pid = fork();
    if (pid == 0)
    {
        // Child process - restore default signal handling
        setup_child_signals();
        
        execve(cmd_path, args, envp);
        perror("execve");
        exit(127);
    }
    else if (pid > 0)
    {
        // Parent process
        waitpid(pid, &status, 0);
        free(cmd_path);
        ft_free_matrix(envp);
        return WEXITSTATUS(status);
    }
    else
    {
        // Fork failed
        perror("fork");
        free(cmd_path);
        ft_free_matrix(envp);
        return 1;
    }
}

// ========== REDIRECTION HANDLING ==========

int handle_heredoc(t_token *redir_token)
{
    char *delimiter;
    char *line;
    int temp_fd;
    char temp_filename[] = "/tmp/.minishell_heredoc_XXXXXX";
    
    if (!redir_token || !redir_token->next || !redir_token->next->value)
        return -1;
    
    delimiter = redir_token->next->value;
    
    // Create unique temporary file
    temp_fd = mkstemp(temp_filename);
    if (temp_fd == -1)
    {
        perror("mkstemp");
        return -1;
    }
    
    // Read lines until delimiter is found
    while (1)
    {
        ft_putstr_fd("heredoc> ", STDOUT_FILENO);
        line = get_next_line(STDIN_FILENO);
        
        if (!line)  // EOF (Ctrl+D)
            break;
        
        // Check if line matches delimiter (with newline handling)
        if (ft_strncmp(delimiter, line, ft_strlen(delimiter)) == 0 && 
            (line[ft_strlen(delimiter)] == '\n' || line[ft_strlen(delimiter)] == '\0'))
        {
            free(line);
            break;
        }
        
        // Write line to temp file
        write(temp_fd, line, ft_strlen(line));
        free(line);
    }
    
    // Close write fd and reopen for reading
    close(temp_fd);
    temp_fd = open(temp_filename, O_RDONLY);
    if (temp_fd == -1)
    {
        perror("open");
        unlink(temp_filename);  // Clean up temp file
        return -1;
    }
    
    // Clean up temp file (will be deleted when fd is closed)
    unlink(temp_filename);
    
    return temp_fd;
}

int handle_input_redirection(t_token *redir_token)
{
    int fd;
    
    if (!redir_token || !redir_token->next)
        return -1;
    
    if (redir_token->type == TOKEN_HEREDOC)
    {
        fd = handle_heredoc(redir_token);
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

int handle_output_redirection(t_token *redir_token)
{
    int fd;
    int flags;
    
    if (!redir_token || !redir_token->next)
        return -1;
    
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

int setup_redirections(t_token *tokens, int *saved_stdin, int *saved_stdout)
{
    t_token *current;
    
    // Save original file descriptors FIRST
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
            if (handle_input_redirection(current) == -1)
                return -1;
        }
        else if (current->type == TOKEN_REDIR_OUT || current->type == TOKEN_APPEND)
        {
            if (handle_output_redirection(current) == -1)
                return -1;
        }
        current = current->next;
    }
    
    return 0;
}

void restore_redirections(int saved_stdin, int saved_stdout)
{
    // Restore original file descriptors
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
    
    assignment_count = 0;
    current = tokens;
    
    while (current)
    {
        if (current->type == TOKEN_WORD && is_assignment(current->value))
        {
            char *key;
            char *value;
            char *expanded_value;
            
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
    int assignment_count;
    int saved_stdin;
    int saved_stdout;
    
    if (!tokens)
        return 0;
    
    // 1. Handle assignments first (before expansion)
    assignment_count = execute_assignments(tokens, env);
    
    // 2. Expand variables and split tokens (creates new token list)
    expanded_tokens = expand_token_list_copy(tokens, *env, exit_status);
    if (!expanded_tokens)
        return (assignment_count > 0) ? 0 : 1;
    
    // 3. Classify the command type AFTER expansion
    cmd_type = classify_command(expanded_tokens, *env);
    
    // 4. If only assignments, return success
    if (cmd_type == CMD_ASSIGNMENT)
    {
        free_token_list(expanded_tokens);
        return 0;
    }
    
    // 5. Setup redirections (saves original stdin/stdout)
    if (setup_redirections(expanded_tokens, &saved_stdin, &saved_stdout) == -1)
    {
        free_token_list(expanded_tokens);
        return 1;
    }
    
    // 6. Convert to args array
    args = tokens_to_args_array(expanded_tokens);
    if (!args)
    {
        restore_redirections(saved_stdin, saved_stdout);
        free_token_list(expanded_tokens);
        return (assignment_count > 0) ? 0 : 1;
    }
    
    // 7. Execute based on command type
    if (cmd_type == CMD_BUILTIN)
        result = execute_builtin_command(args, env);
    else if (cmd_type == CMD_EXTERNAL)
        result = execute_external_command(args, *env);
    else
    {
        ft_printf("minishell: %s: command not found\n", args[0]);
        result = 127;
    }
    
    // 8. Restore redirections BEFORE cleanup
    restore_redirections(saved_stdin, saved_stdout);
    
    // 9. Cleanup
    ft_free_matrix(args);
    free_token_list(expanded_tokens);
    
    return result;
}

// ========== PIPELINE EXECUTION ==========

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
    
    // Save original stdin/stdout
    saved_stdin = dup(STDIN_FILENO);
    saved_stdout = dup(STDOUT_FILENO);
    
    if (pipe(pipefd) == -1)
    {
        perror("pipe");
        close(saved_stdin);
        close(saved_stdout);
        return 1;
    }
    
    left_pid = fork();
    if (left_pid == 0)
    {
        // Left child: output to pipe
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);
        exit(execute_tree(tree->left, env, exit_status));
    }
    else if (left_pid == -1)
    {
        perror("fork");
        close(pipefd[0]);
        close(pipefd[1]);
        close(saved_stdin);
        close(saved_stdout);
        return 1;
    }
    
    right_pid = fork();
    if (right_pid == 0)
    {
        // Right child: input from pipe
        close(pipefd[1]);
        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[0]);
        exit(execute_tree(tree->right, env, exit_status));
    }
    else if (right_pid == -1)
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
    
    // Parent: close pipe and wait for children
    close(pipefd[0]);
    close(pipefd[1]);
    
    waitpid(left_pid, NULL, 0);
    waitpid(right_pid, &status, 0);
    
    // Restore original stdin/stdout
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
    
    // Only execute right side if left succeeded
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
    
    // Only execute right side if left failed
    if (left_result != 0 && tree->right)
        return execute_tree(tree->right, env, left_result);
    
    return left_result;
}

// ========== PARENTHESES EXECUTION ==========

int execute_parentheses(t_tree *tree, t_env **env, int exit_status)
{
    if (!tree || !tree->left)
        return 1;
    
    // Execute the content inside parentheses
    return execute_tree(tree->left, env, exit_status);
}

// ========== MAIN EXECUTION FUNCTION ==========

int execute_tree(t_tree *tree, t_env **env, int exit_status)
{
    if (!tree || !env)
        return 1;
    
    if (tree->type == NODE_COMMAND)
        return execute_simple_command(tree->tokens, env, exit_status);
    else if (tree->type == NODE_PIPE)
        return execute_pipeline(tree, env, exit_status);
    else if (tree->type == NODE_AND)
        return execute_logical_and(tree, env, exit_status);
    else if (tree->type == NODE_OR)
        return execute_logical_or(tree, env, exit_status);
    else if (tree->type == NODE_PAREN)
        return execute_parentheses(tree, env, exit_status);
    else
    {
        ft_printf("Unknown node type: %d\n", tree->type);
        return 1;
    }
}
