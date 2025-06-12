/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elorente <elorente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 15:43:14 by alepinto          #+#    #+#             */
/*   Updated: 2025/06/12 16:02:15 by elorente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

# include <sys/wait.h>
# include <unistd.h>
# include <fcntl.h>
# include <signal.h>
# include "structs.h"

//enviroment.c

char			*get_env_value(t_env *env, const char *key);
int				env_key_exists(t_env *env, const char *key);
void			update_env_value(t_env *entry, const char *value);
void			env_set_value(t_env **env, const char *key, const char *value);

//enviroment_v2.c

void			env_unset_key(t_env **env, const char *key);
int				parse_assignment(const char *assignment,
					char **key, char **value);
int				is_valid_identifier(const char *str);
int				is_assignment(const char *str);
char			*get_env_name(char *dest, const char *src);

//enviroment_v3.c

void			env_add(const char *value, t_env *env);
t_env			*create_env_node(const char *key, const char *value);
t_env			*create_env_node_from_string(const char *env_str);
t_env			*env_from_array(char **envp);
int				count_env_vars(t_env *env);

//enviroment_v4.c

char			*build_env_string(const char *key, const char *value);
char			**env_to_array(t_env *env);
void			free_env_list(t_env *head);
void			print_env_list(t_env *env);
t_env			*copy_env_list(t_env *original);

//expander.c

char			*extract_variable_name(const char *str, int *consumed_len);
char			*return_var_segment(const char *str, int i, int *consumed_len);
char			*resolve_variable_value(const char *var_name,
					t_env *env, int exit);
char			*join_and_free(char *s1, char *s2);
char			*append_char_and_free(char *str, char c);

//expander_v2.c

int				has_wildcard(const char *str);
char			*extract_directory_path(const char *pattern);
char			*extract_filename_pattern(const char *pattern);
int				pattern_match(const char *pattern, const char *filename);
int				should_include_file(const char *filename, const char *pattern);

//expander_v3.c

int				handle_match(char ***matches, int *count,
					int *capacity, char *name);
char			**collect_matching_files(const char *dir_path,
					const char *file_pattern);
void			swap_if_needed(char **array, int a, int b);
void			sort_string_array(char **array);
char			**build_full_paths(const char *dir_path, char **filenames);

//expander_v4.c

void			remove_empty_string(char **words, int index);
void			replace_tabs_with_spaces(char *str);
char			**split_expanded_value(const char *expanded_value);
void			append_token(t_token **head, t_token **tail,
					t_token *new_token);
t_token			*create_tokens_from_words(char **words, t_token_type base_type);

//expander_v5.c

t_token			*copy_literal_token(t_token *original);
t_token			*expand_assignment_token(t_token *original,
					t_env *env, int status);
t_token			*handle_wildcard_token(char *expanded, t_token_type type);
t_token			*split_and_create_tokens(char *expanded, t_token_type type);
t_token			*expand_and_split_token_copy(t_token *original,
					t_env *env, int exit_status);

//expander_v6.c

void			append_token_list(t_token **head, t_token **tail,
					t_token *new_tokens);
void			handle_expanded_token(t_token *expanded,
					t_token **head, t_token **tail);
int				process_assignment(const char *assignment, t_env **env);
char			**expand_wildcard_pattern(const char *pattern);
int				needs_splitting(const char *str);

//expander_v7.c

char			*build_path(const char *dir_path, const char *filename);
char			*append_variable_expansion(char *result, t_expand_ctx *ctx);
char			*append_raw_segment(char *result, const char *line, int end);
char			*expand_variable_segment(char *result, t_expand_ctx *ctx);
char			*append_segment(char *res, const char *line,
					int start, int end);

//expander_v8.c

t_token			*expand_token_list_copy(t_token *original,
					t_env *env, int status);
int				should_expand_segment(int i, int start);
int				expand_and_append(char **res, const char *line,
					int *start, int i);
int				handle_dollar_loop(const char *line, char **res,
					t_expand_ctx *ctx);
char			*expand_string(const char *line, t_env *env, int exit_status);

//executor.c

int				is_builtin_name(const char *cmd);
char			*get_first_command_word(t_token *tokens);
t_command_type	classify_command(t_token *tokens, t_env *env);
int				execute_echo_builtin(char **args);
int				execute_cd_builtin(char **args, t_env **env);

//executor_v2.c

int				execute_export_builtin(char **args, t_env **env);
int				handle_exit_result(char **args);
int				execute_builtin_command(char **args, t_env **env);
int				should_include_token(t_token *token, t_token *prev);
int				count_word_tokens(t_token *tokens);

//executor_v3.c

int				fill_args_array(char **args, t_token *tokens);
char			**tokens_to_args_array(t_token *tokens);
char			*check_absolute_path(const char *cmd);
char			*search_in_path_dirs(const char *cmd, char **path_dirs);
char			*find_command_in_path(const char *cmd, t_env *env);

//executor_v4.c

int				fork_and_execute(char *cmd_path, char **args, char **envp);
int				execute_external_command(char **args, t_env *env);
int				create_heredoc_temp_file(void);
int				ft_mkstemp(char *buffer, size_t size);
int				write_heredoc_content(int temp_fd, char *delimiter,
					t_env *env, int exit);

//executor_v5.c

int				handle_heredoc(t_token *redir_token, t_env *env,
					int exit_status);
int				setup_input_redirection(t_token *redir_token, t_env *env,
					int exit_status);
int				setup_output_redirection(t_token *redir_token);
int				setup_redirections(t_token *tokens, t_env *env,
					int exit_status, t_redir_ctx *ctx);
void			restore_redirections(int saved_stdin, int saved_stdout);

//executor_v6.c

int				execute_assignments(t_token *tokens, t_env **env);
int				prepare_arguments(t_exec_context *ctx);
void			execute_final_command(t_exec_context *ctx, t_env **env);
int				execute_simple_command(t_token *tokens, t_env **env,
					int exit_status);
int				setup_pipeline_fds(int pipefd[2]);

//executor_v7.c

int				fork_left_process(t_tree *tree, t_env **env, int exit_status,
					int pipefd[2]);
int				fork_right_process(t_tree *tree, t_env **env,
					int exit_status, int pipefd[2]);
int				close_fds_restore(int in, int out);
int				handle_fork_error(t_pipeline_ctx *ctx, int kill_left);
int				finalize_pipeline(t_pipeline_ctx *ctx);

//executor_v8.c

int				execute_pipeline(t_tree *tree, t_env **env, int exit_status);
int				execute_logical_and(t_tree *tree, t_env **env, int exit_status);
int				execute_logical_or(t_tree *tree, t_env **env, int exit_status);
int				execute_parentheses(t_tree *tree, t_env **env, int exit_status);
int				execute_tree(t_tree *tree, t_env **env, int exit_status);

char			*expand_exit_status(char *res, int exit_status);

#endif