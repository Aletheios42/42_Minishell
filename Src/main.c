#include "../Inc/minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "../libft/libft.h"



int main(int ac, char **av, char **env) {
  (void)ac,
  (void)av;
  (void)env;
  char *input;

  t_tree* data_tree;



  while (42) {
    input = readline("prompt $ ");
    add_history(input);


    data_tree = parser(input);
    ft_print_syntax_tree(data_tree, 3);
    (void)data_tree;
  }
  return 0;
}

// int main(int ac, char **av, char **env) {
//   (void)ac,
//   (void)av;
//   char *input;
//   t_token* tokens;
//   t_tree* data_tree;
//
//   while (42) {
//     input = readline("prompt $ ");
//     add_history(input);
//
//     tokens = lexer(input);
//     data_tree = parser(tokens);
//     // ft_print_tree(data_tree, 0);
//     executor(data_tree, env);
//   }
//   return 0;
// }
