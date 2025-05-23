#ifndef EXECUTOR_H
#define EXECUTOR_H

#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include "structs.h"

// ========== MAIN EXECUTION ==========
int execute_tree(t_tree *tree, t_env **env, int exit_status);

// ========== COMMAND EXECUTION ==========
int execute_simple_command(t_token *tokens, t_env **env, int exit_status);
int execute_pipeline(t_tree *tree, t_env **env, int exit_status);
int execute_logical_and(t_tree *tree, t_env **env, int exit_status);
int execute_logical_or(t_tree *tree, t_env **env, int exit_status);
int execute_parentheses(t_tree *tree, t_env **env, int exit_status);

// ========== BUILTIN COMMANDS ==========
int is_builtin_command(const char *cmd);
int execute_builtin_command(char **args, t_env **env);

// ========== EXTERNAL COMMANDS ==========
char *find_command_in_path(const char *cmd, t_env *env);
int execute_external_command(char **args, t_env *env);

// ========== TOKEN PROCESSING ==========
int count_command_tokens(t_token *tokens);
char **tokens_to_args_array(t_token *tokens);

// ========== REDIRECTION HANDLING ==========
int setup_redirections(t_token *tokens, int *saved_stdin, int *saved_stdout);
int handle_input_redirection(t_token *redir_token);
int handle_output_redirection(t_token *redir_token);

// ========== ENVIRONMENT FUNCTIONS ==========
char *get_env_value(t_env *env, const char *key);
void env_set_value(t_env **env, const char *key, const char *value);
void env_unset_key(t_env **env, const char *key);
int env_key_exists(t_env *env, const char *key);
t_env *env_from_array(char **envp);
char **env_to_array(t_env *env);
void free_env_list(t_env *head);

// ========== ASSIGNMENT HANDLING ==========
int is_assignment(const char *str);
int parse_assignment(const char *assignment, char **key, char **value);
int is_valid_identifier(const char *str);
int process_assignment(const char *assignment, t_env **env);
int execute_assignments(t_token *tokens, t_env **env);

// ========== ENVIRONMENT UTILITIES ==========
t_env *create_env_node(const char *key, const char *value);
t_env *create_env_node_from_string(const char *env_str);
int count_env_vars(t_env *env);
void print_env_list(t_env *env);
t_env *copy_env_list(t_env *original);

// ========== EXPANSION FUNCTIONS ==========
t_token *expand_token_list_copy(t_token *original_tokens, t_env *env, int exit_status);
char *expand_string(const char *str, t_env *env, int exit_status);
char *extract_variable_name(const char *str, int *consumed_len);
char *resolve_variable_value(const char *var_name, t_env *env, int exit_status);

// ========== TOKEN SPLITTING ==========
int needs_splitting(const char *str);
char **split_expanded_value(const char *expanded_value);
t_token *create_tokens_from_words(char **words, t_token_type base_type);
t_token *expand_token(t_token *token, t_env *env, int exit_status);

// ========== TOKEN MANAGEMENT ==========
t_token *create_token(char *value, t_token_type type);
void free_token_list(t_token *head);

#endif
