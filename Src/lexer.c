#include "../libft/libft.h"
#include "../Inc/minishell.h"

//ARREGLAR LAS COMPARACIONES
t_token_type set_token(char *tok) {
  if (!ft_strncmp(tok , "|", 2))
    return TOKEN_PIPE;
  else if (!ft_strncmp(tok , "<", 2))
    return TOKEN_REDIR_IN;
  else if (!ft_strncmp(tok , ">", 2))
    return TOKEN_REDIR_OUT;
  else if (!ft_strncmp(tok , ">>", 3))
    return TOKEN_APPEND;
  else if (!ft_strncmp(tok , "<<", 3))
    return TOKEN_HEREDOC;
  else if (!ft_strncmp(tok , "&&", 3))
    return TOKEN_AND;
  else if (!ft_strncmp(tok , "||", 3))
    return TOKEN_OR;
  else if (!ft_strncmp(tok , "(", 2))
    return TOKEN_OPEN_PAREN;
  else if (!ft_strncmp(tok , ")", 2))
    return TOKEN_CLOSE_PAREN;
  else if (!ft_strncmp(tok , "", 1))
    return TOKEN_CLOSE_PAREN;
  else
    return TOKEN_WORD;
}

t_token* lexer(char *input) {
  t_token *tokens = NULL;
  t_token *aux_tokens = NULL;
  t_token *new_node = NULL;
  char *tok;
  
  tok = ft_strtok(input, " ");
  while (tok) {
    new_node = (t_token *)malloc(sizeof(t_token));
    if (!new_node)
      return NULL;
    new_node->value = tok;
    new_node->type = set_token(tok);
    new_node->next = NULL;
    
    if (!tokens)
      tokens = new_node;
    else
      aux_tokens->next = new_node;
    aux_tokens = new_node;
    
    tok = ft_strtok(NULL, " ");
  }
  ft_printf("%T", tokens);
  return tokens;
}
