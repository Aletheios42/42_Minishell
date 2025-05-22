#ifndef EXECUTOR_H
#define EXECUTOR_H

#include <sys/wait.h>
#include <unistd.h>
#include "structs.h"

// Main execution function
int execute_tree(t_tree *tree, t_env **env, t_local_vars **locals, int exit_status);

// Environment functions
char *get_env_value(t_env *env, const char *key);
t_env *env_from_array(char **envp);
void free_env_list(t_env *head);

// Local variables functions
void local_var_set(t_local_vars **locals, char *key, char *value);
char *local_var_get(t_local_vars *locals, char *key);
void local_var_unset(t_local_vars **locals, char *key);
void free_local_vars(t_local_vars *locals);

// Expansion functions
void expand_token_list(t_token *tokens, t_env *env, t_local_vars *locals, int exit_status);

// Helper functions for string operations
char *ft_strjoin_char(char *str, char c);


t_token *create_token(char *value, t_token_type type);
void free_token_list(t_token *head);
#endif
