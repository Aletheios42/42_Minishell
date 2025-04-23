#include "minishell.h"
#include <readline/readline.h>
#include <readline/history.h>

int main(int ac, char **av, char **env) {

  char *input;
  (void)env;
  while (1) {
    input = readline("prompt>");
  }
  return 0;
}
