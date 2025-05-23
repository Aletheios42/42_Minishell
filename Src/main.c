#include "../Inc/minishell.h"
#include "../Inc/signals.h"
#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

// ========== CLEANUP FUNCTIONS ==========

void cleanup_resources(t_env *env)
{
    if (env)
        free_env_list(env);
    
    // Clear readline history
    rl_clear_history();
}

// ========== MAIN EXECUTION FLOW ==========

int execute_input(char *input, t_env **env, int last_exit_status)
{
    t_token *tokens;
    t_tree *syntax_tree;
    int exit_status;
    
    if (!input || !*input)
        return last_exit_status;
    
    // Tokenize input
    tokens = lexer(input);
    if (!tokens)
    {
        ft_putendl_fd("minishell: syntax error", 2);
        return 2;
    }
    
    // Parse tokens into syntax tree
    syntax_tree = parser(tokens);
    if (!syntax_tree)
    {
        // Parser handles its own error messages
        free_token_list(tokens);  // Free tokens if parser fails
        return 2;
    }
    
    // Execute the syntax tree
    exit_status = execute_tree(syntax_tree, env, last_exit_status);
    
    // Cleanup
    free_syntax_tree(syntax_tree);
    // Don't free tokens here - syntax_tree owns them
    
    return exit_status;
}

// ========== MAIN FUNCTION ==========

int main(int ac, char **av, char **envp)
{
    t_env *env;
    char *input;
    int exit_status;
    int is_command_mode;
    
    // Initialize environment
    env = env_from_array(envp);
    if (!env)
    {
        ft_putendl_fd("minishell: failed to initialize environment", 2);
        return 1;
    }
    
    // Determine execution mode
    is_command_mode = (ac >= 3 && ft_strcmp(av[1], "-c") == 0);
    exit_status = 0;
    
    // Setup signal handling for interactive mode
    if (!is_command_mode)
        setup_signals();
    
    // Main execution loop
    while (1)
    {
        // Get input based on mode
        if (is_command_mode)
        {
            input = ft_strdup(av[2]);
        }
        else
        {
            input = readline("minishell$ ");
            if (!input)  // EOF (Ctrl+D)
            {
                ft_putendl_fd("exit", 1);
                break;
            }
        }
        
        // Skip empty input
        if (!input || !*input)
        {
            free(input);
            if (is_command_mode)
                break;
            continue;
        }
        
        // Add to history in interactive mode
        if (!is_command_mode)
            add_history(input);
        
        // Execute the input
        exit_status = execute_input(input, &env, exit_status);
        
        // Cleanup input
        free(input);
        
        // Break after one iteration in command mode
        if (is_command_mode)
            break;
    }
    
    // Cleanup and exit
    cleanup_resources(env);
    return exit_status;
}
