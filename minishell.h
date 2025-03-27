/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzolotar <mzolotar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 08:50:00 by mzolotar          #+#    #+#             */
/*   Updated: 2025/03/07 14:00:34 by mzolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H

// #➵⤐──╌╌➣⋆➣╌╌──⤏➵•➵⤐──╌╌➣⋆➣╌╌──Includes:──╌╌➣⋆➣╌╌──⤏➵•➵⤐──╌╌➣⋆➣╌╌➔#

// ✅ todos los includes:

#include "libft/libft.h"

#include <curses.h> // tgetent, tgetflag, tgetnum, tgetstr, tgoto, tputs
#include <dirent.h> // opendir, readdir, closedir
#include <errno.h>  // perror
#include <fcntl.h>  // open
#include <readline/history.h>
#include <readline/readline.h> // readline, rl_clear_history, rl_on_new_line, rl_replace_line, rl_redisplay, add_history
#include <signal.h>            // signal, sigaction, kill
#include <stdio.h>             // printf
#include <stdlib.h>            // malloc, free, getenv
#include <string.h>            // strerror
#include <sys/stat.h>          // stat, lstat, fstat
#include <sys/wait.h>          // wait, waitpid, wait3, wait4
#include <termios.h>           // tcsetattr, tcgetattr
#include <unistd.h> // write, access, read, close, fork, execve, pipe, isatty

#include <stdbool.h> //bool

// #➵⤐──╌╌➣⋆➣╌╌──⤏➵•➵⤐──╌╌➣⋆➣╌╌──Macros  :──╌╌➣⋆➣╌╌──⤏➵•➵⤐──╌╌➣⋆➣╌╌➔#

// ✅ tolos mensajes de error:
//	**ERROR MESSAGES: MINISHELL**
#define SYNTAX_ERR "❌ syntax error\n"

//	**ERROR MESSAGES: SHELL**
//	syntax error near unexpected token	-> bash: syntax error near
// unexpected token `newline', `|',  `>', etc
#define BASH_ERR "bash: syntax error near unexpected token"

// Comando inexistente	-> command: command not found
#define ERR_COMMAND ": command not found\n"

// Error de sintaxis	-> syntax error near unexpected token 'X'
// add ERR_MSG "bash: syntax error near unexpected token "

// Variable inválida en export	-> export: 'var': not a valid identifier
#define ERR_IDENTIF "not a valid identifier"

// Permiso denegado	-> filename: Permission denied
// add ERR_MSG ": permission denied"

// cd a directorio inexistente	-> cd: folder: No such file or directory
#define ERR_F_OR_D                                                             \
  ": no such file or directory" // cd y cat en archivo inexistente

// cat en archivo inexistente	-> cat: file: No such file or directory
#define ERR_F_OR_D                                                             \
  ": no such file or directory" // cd y cat en archivo inexistente

// Redirección sin archivo	-> bash: syntax error near unexpected token
// 'newline' add ERR_MSG

// Archivo no ejecutable	-> file: Exec format error
// add ERR_MSG

// #➵⤐─╌➣⋆➣╌─⤏➵•➵⤐─╌➣⋆➣╌╌──Global Variables :──╌╌➣⋆➣╌─⤏➵•➵⤐──╌╌➣⋆➣╌╌➔#

// #➵⤐──╌╌➣⋆➣╌─⤏➵•➵⤐──╌╌➣⋆➣╌╌──Enums :──╌╌➣⋆➣╌╌⤏➵•➵⤐──╌╌➣⋆➣╌╌➔#

typedef enum {
  temp,
  command,
  argument,
  redir_l_u,
  redir_l_d,
  redir_r_u,
  redir_r_d,
  pipe_type,
  path_name,
  heredoc_delimiter
} t_type;

/**
temp 				->  temporal por defecto
command				->  command
argument 			->  argument
redir_l_u   		->  redirection_left_unic 		== '<'
redir_l_d   		->  redirection_left_double 	== '<<'
redir_r_u   		->  redirection_right_double 	== '>'
redir_r_d   		->  redirection_right_double 	== '>>'
pipe_type 			->  pipe
path_name			->  patern_name (echo hello > echo.out !=
echo some more data > ../../over.here) heredoc_delimiter 	->  (<< LIMIT)
*/

// #➵⤐──╌╌➣⋆➣╌─⤏➵•➵⤐──╌╌➣⋆➣╌╌──Structures  :──╌╌➣⋆➣╌╌⤏➵•➵⤐──╌╌➣⋆➣╌╌➔#

// de los dos 🔹 ✒ REVISAR:
typedef struct s_tokens {
  char *content;    // *str con datos
  t_type type;      // **type: command, argument, redir, etc
  bool own_command; // comando propio (echo, cd, pwd, export, unset, env, exit)
                    // o del sistema . ver subject minishell
  bool expansion;   // 🔹 ✒ Gestionar la expansión o no //revisar esto
  int position;     // posicion del nodo en la lista	(revisar si hace falta)
  struct s_tokens *next;
} t_tokens;

typedef struct s_sentences {
  char *cmd;
  int infile;
  int outfile;
  char *binOp;
  struct s_sentence *next;
} t_sentence;

// ✅s_metachars
typedef struct s_metachars {

  // char **allow_oper;
  // //	<< >>
  // //✅_se usa char **allow_metachar;
  // //	< > |
  // //✅_se usa
  char **allow_op; // < > | << >>
                   // //✅_se usa + //🚩 revisar: <<< (en los tokens( ->
                   // synt_error)
  char **var;      //$

  // char **not_allow_metachar;
  // //	\n ; : \t & ) ( # !
  // //✅_se usa char **not_allow_bracket;
  // //	[ { ] }
  // //✅_se usa char **is_metacharacter;
  // //	\t ' ' \n | & ; ( ) < >
  // //🚩 revisar si se usa

  // char **not_allow_num_redir;
  // //<<- << <> >> < >
  // //✅_se usa char **not_allow_other_redir;
  // // " <<<,<<<, <<-,<<-, 2>, 2>>"    (con espacio antes)		//✅_se
  // usa char **not_allow_oper;
  // // || ;; >| $|
  // //✅_se usa
  char **unexpected_token; //

  // char **not_allow_word;
  // // alias...
  // //🚩 revisar si se usa -> check_reserved_words() char *word;
  // //🚩 revisar si se usa + //reservado para meter word en las comprobaciones
  // de variables y check_reserved_word (meta->not_allow_word) char *variable;

  int i_current_index; // ✅_se usa reservado para el uso de check_spaces_line()
  int j_new_index; // ✅_se usa reservado para el uso de check_spaces_line()

  bool in_single_quote; // ✅_se usa reservado para el uso de
                        // check_spaces_line()
  bool in_double_quote; // ✅_se usa reservado para el uso de
                        // check_spaces_line()

} t_metachars;

// #➵⤐╌╌➣⋆➣╌─⤏➵•➵⤐╌╌➣⋆➣╌╌─Function Prototypes : :──➣⋆➣╌⤏➵•➵⤐╌╌➣⋆➣╌╌➔#

//-----------------test_utils_borrar──╌╌➣⋆➣╌╌──⤏➵•➵⤐──╌╌➣⋆➣╌╌➔#
// ✅ test_utils.c (borrar)
void print_metachars(const t_metachars *meta);
void print_string_array(const char *label, char **array);
void print_linked_list(t_tokens *head, int fd);
void print_type_list(t_tokens *tokens, int fd);
void print_content_list(t_tokens *tokens, int fd);
void print_list(t_tokens *tokens, int fd);
void print_split_before_list(char **token, int fd);
void print_command_line(char *line, int fd);
void print_all_test(char *line, char **token, t_tokens *tokens, int fd);

// ✅test_funcionts.c (borrar)
void test_minishell_loop(char **envp);
//-----------------test_utils_end──╌╌➣⋆➣╌╌──⤏➵•➵⤐──╌╌➣⋆➣╌╌➔#

// 🔹 ✒ REVISAR:main.c (/5):
// int main (int argc, char *argv[], char **envp);

// ✅utils.c (/5)
int open_fd(char *pathern, int flags, mode_t mode);
char *get_line(void);
t_tokens *ft_find_node_n_position(t_tokens *head, int position);
t_metachars *init_meta_struct(void);

//------quotes_utils─╌╌➣⋆➣╌╌──⤏➵•➵⤐──╌╌➣⋆➣╌╌➔#
// ✅ckeck_quotes_utils.c (/5):
bool is_quotes_present(const char *str);
bool quotes_is_open(const char *str);
bool is_even_quotes(const char *str);
bool char_is_quoted(const char *str, char character, const char *position);
bool oper_is_quoted(const char *str, const char *operator_str,
                    const char *position);
void toggle_quotes(char c, bool *in_single_quote, bool *in_double_quote);
bool is_operator(const char *str, const char *op);
void update_quote_status(const char c, t_metachars *meta);
bool check_operator(const char *str, char **operators);
bool check_metacharacter(const char *str, char **character);
//------quotes_utils_end──╌╌➣⋆➣╌╌──⤏➵•➵⤐──╌╌➣⋆➣╌╌➔#

//--------------check_spaces_line──╌╌➣⋆➣╌╌──⤏➵•➵⤐──╌╌➣⋆➣╌╌➔#

// ✅check_spaces_line_utils.c (/5)
bool needs_space_before(const char *line, int i);
bool needs_space_after(const char *line, int i, int op_len);
int count_extra_spaces(const char *line, t_metachars *meta);
int count_spaces_for_ops(const char *line, t_metachars *meta);
char *allocate_new_line(const char *line, t_metachars *meta);

// ✅check_spaces_line.c (/5)
void copy_operator(char *new_line, t_metachars *meta, const char *line,
                   int op_len);
void process_operators(const char *line, char *new_line, t_metachars *meta);
void process_line(const char *line, char *new_line, t_metachars *meta);
void replace_line(char **line, char *new_line);
void check_spaces_line(char **line, t_metachars *meta);
//-----------check_spaces_line_end──╌╌➣⋆➣╌╌──⤏➵•➵⤐──╌╌➣⋆➣╌╌➔#

//--tokeniser──╌╌➣⋆➣╌╌──⤏➵•➵⤐──╌╌➣⋆➣╌╌➔#
// ✅ft_split_with_quotes.c (/5)
int count_tokens(const char *sts, t_metachars *meta);
char *extract_token(const char *sts, t_metachars *meta);
char **ft_split_with_quotes(char const *sts, t_metachars *meta);

// ✅minishell_loop.c (/5)
void minishell_loop(char **envp);
//--tokeniser_end──╌╌➣⋆➣╌╌──⤏➵•➵⤐──╌╌➣⋆➣╌╌➔#

//--check_type_tokens──╌╌➣⋆➣╌╌──⤏➵•➵⤐──╌╌➣⋆➣╌╌➔#
// ✅check_type_tokens_utils.c (/5)
bool check_pipe(t_tokens *current);
bool is_redir(t_tokens *current);
t_type check_redir(t_tokens *current);
bool check_command(t_tokens *current, t_tokens *head);
bool check_argument(t_tokens *current, t_tokens *head);

// ✅check_type_tokens.c (/5)
bool check_own_command(t_tokens *current);
void assign_redirection_type(t_tokens *current, t_tokens *head);
bool assign_token_type(t_tokens *current, t_tokens *head);
void process_tokens(t_tokens *tokens, bool *has_temp_tokens);
void check_type_tokens(t_tokens *tokens);
//--check_type_tokens_end──╌╌➣⋆➣╌╌──⤏➵•➵⤐──╌╌➣⋆➣╌╌➔#

// ✅check_syntax_error_token.c (/5)
bool check_open_heredoc_token(t_tokens *tokens); // revisar wtf
bool check_open_pipe_token(t_tokens *tokens);
bool check_bash_error_token(t_tokens *tokens);

// ✅check_expand_var.c (/5)
bool is_valid_var(const char *str);
bool check_extract_and_expand_var_token(t_tokens *tokens, t_metachars *meta);

// ✅quit_quotes_token.c  (/5)
bool is_outer_quote(char *content, int i, bool *in_single_quote,
                    bool *in_double_quote);
bool is_inner_quote(char *content, int i);
void quit_quotes_token(t_tokens *current, t_metachars *meta);
void check_and_quit_quotes_token(t_tokens *tokens, t_metachars *meta);

// ✅free_utils.c (/5)
void free_split_strs(char **split_strs);
void free_tokens_list(t_tokens **tokens);
void free_meta_struct(t_metachars *meta);
void free_mini_loop(char *line, char **token, t_tokens **tokens,
                    t_metachars *meta);

// ✅list_utils.c (/5)
t_tokens *find_last_node(t_tokens *tokens);
// static void initialize_node(t_tokens *tokens, char *content, int position);
// static void add_node(t_tokens **tokens, char *content, int position);
// void	init_list(t_tokens **tokens, char **split_str);
void init_list(t_tokens **tokens, char **split_str);

#endif
