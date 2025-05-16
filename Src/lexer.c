#include "../libft/libft.h"
#include "../Inc/minishell.h"


t_token_type set_token_type(char *input) {
  char *delims[] = {"<", "<<", ">>", ">", "(", ")", "\"", "\x27", "&&", "|", "||", ";", "&", NULL};
  int i;

  i = -1;
    while (delims[++i])
            ft_strncmp(input, delims[i], ft_strlen(delims[i]));
    return i;
}

t_token *new_token(char *value) {
    t_token *node = NULL;

    node = (t_token *)malloc(sizeof(t_token));
    if (!node)
        return NULL;
    ft_memset(node, 0, sizeof(t_token));
    node->value = value;
    node->type = set_token_type(value);
    node->next = NULL;
    return node;

}

int pass_metachar(char *str) {
    int i;

    i = 0;
    while ()
    {}
    return i;
}

t_token* lexer(char *input) {
  char * tok = "inicializando";
  char * current = "inicializando";
  char *delims[] = {"<", "<<", ">>", ">", "(", ")", "\"", "\x27", "&&", "|", "||", ";", "&", NULL};
  t_token  *tokens;

  while (tok) {
    tok = ft_strarraystr(input, delims);
    current = ft_substr(input, 0, tok - current);
    printf("%s\n", current);
    //settear token
    tokens = new_token(current);
    // pasarlo
    tok += pass_metachar(tok);
    input = tok;
    printf("%s\n", tok);
    
  }
  return NULL;
}
