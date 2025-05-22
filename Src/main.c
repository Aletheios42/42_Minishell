#include "../Inc/minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "../libft/libft.h"

int execute_command_mode(char *command, t_env *env)
{
    t_token *tokens;
    t_tree *data_tree;
    int exit_status;
    t_local_vars *local_vars;
    
    local_vars = NULL;
    tokens = lexer(command);
    ft_printf("Tokens: ");
    ft_print_token_list(tokens);
    ft_printf("\n");
    if (!tokens)
    {
        ft_putendl_fd("Error: bad input", 2);
        return (1);
    }
    data_tree = parser(tokens);
    exit_status = 0;
    if (data_tree)
        exit_status = execute_tree(data_tree, &env, &local_vars, exit_status);
    free_syntax_tree(data_tree);
    free_local_vars(local_vars);
    return (exit_status);
}

void run_shell_loop(t_env *env)
{
    char *input;
    t_token *tokens;
    t_tree *data_tree;
    int last_exit_status;
    t_local_vars *local_vars;  // ← Variable persistente
    
    last_exit_status = 0;
    local_vars = NULL;         // ← Inicializar una sola vez
    
    while (1)
    {
        input = readline("minishell$ ");
        if (!input)
            break;
        if (*input)
            add_history(input);
        tokens = lexer(input);
        if (!tokens)
        {
            free(input);
            continue;
        }
        data_tree = parser(tokens);
        if (data_tree)
            // ← Pasar SIEMPRE las mismas local_vars
            last_exit_status = execute_tree(data_tree, &env, &local_vars, last_exit_status);
        free_syntax_tree(data_tree);
        free(input);
    }
    
    // ← Limpiar al salir del shell
    free_local_vars(local_vars);
}

int main(int ac, char **av, char **envp)
{
    t_env *env;
    int exit_status;
    
    env = env_from_array(envp);
    if (!env)
    {
        ft_putendl_fd("Error: Failed to initialize environment", 2);
        return (1);
    }
    if (ac >= 3 && ft_strcmp(av[1], "-c") == 0)
    {
        exit_status = execute_command_mode(av[2], env);
        free_env_list(env);
        return (exit_status);
    }
    run_shell_loop(env);
    free_env_list(env);
    return (0);
}
