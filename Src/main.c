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
    
    rl_clear_history();
}

// ========== MAIN EXECUTION FLOW ==========

int execute_input(char *input, t_env **env, int last_exit_status, 
                 int *should_exit)
{
    t_token *tokens;
    t_tree *syntax_tree;
    int exit_status;
    
    if (!input || !*input)
        return last_exit_status;
    
    tokens = lexer(input);
    if (!tokens)
    {
        ft_putendl_fd("minishell: syntax error", 2);
        return 2;
    }
    
    syntax_tree = parser(tokens);
    if (!syntax_tree)
    {
        free_token_list(tokens);
        return 2;
    }
    
    exit_status = execute_tree(syntax_tree, env, last_exit_status);
    
    if (exit_status >= 1000)
    {
        *should_exit = 1;
        if (exit_status == 1000)
            exit_status = 0;
        else if (exit_status == 1001)
            exit_status = 1;
        else if (exit_status == 1255)
            exit_status = 255;
        else
            exit_status = (exit_status - 1000) % 256;
    }
    
    free_syntax_tree(syntax_tree);
    
    return exit_status;
}

// ========== MAIN FUNCTION ==========

int main(int ac, char **av, char **envp)
{
    t_env *env;
    char *input;
    int exit_status;
    int is_command_mode;
    int should_exit;
    
    env = env_from_array(envp);
    if (!env)
    {
        ft_putendl_fd("minishell: failed to initialize environment", 2);
        return 1;
    }
    
    is_command_mode = (ac >= 3 && ft_strcmp(av[1], "-c") == 0);
    exit_status = 0;
    should_exit = 0;
    
    if (!is_command_mode)
        setup_signals();
    
    while (!should_exit)
    {
        if (is_command_mode)
        {
            input = ft_strdup(av[2]);
        }
        else
        {
            input = readline("minishell$ ");
            if (!input)
            {
                ft_putendl_fd("exit", 1);
                break;
            }
        }
        
        if (!input || !*input)
        {
            free(input);
            if (is_command_mode)
                break;
            continue;
        }
        
        if (!is_command_mode)
            add_history(input);
        
        exit_status = execute_input(input, &env, exit_status, &should_exit);
        
        free(input);
        
        if (is_command_mode)
            break;
    }
    
    cleanup_resources(env);
    return exit_status;
}
