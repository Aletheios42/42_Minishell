#include "../libft/libft.h"
#include "../Inc/minishell.h"

DFA set_lexer_DFA(t_token *tokens) {
    static DFA DFA_lexer;
static int transition_matrix[MAX_STATES][MAX_SYMBOLS] = {
    // TOKEN_WORD, TOKEN_PIPE, TOKEN_REDIR_IN, TOKEN_REDIR_OUT, TOKEN_APPEND, TOKEN_HEREDOC, TOKEN_AND, TOKEN_OR, TOKEN_OPEN_PAREN, TOKEN_CLOSE_PAREN, TOKEN_EOF
    {STATE_WORD, STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_IN_PAREN, STATE_ERROR, STATE_ERROR}, // STATE_INITIAL
    {STATE_WORD, STATE_AFTER_OP, STATE_AFTER_OP, STATE_AFTER_OP, STATE_AFTER_OP, STATE_AFTER_OP, STATE_AFTER_OP, STATE_AFTER_OP, STATE_IN_PAREN, STATE_WORD, STATE_ACCEPT}, // STATE_IN_PAREN
    {STATE_IN_DQUOTE, STATE_IN_DQUOTE, STATE_IN_DQUOTE, STATE_IN_DQUOTE, STATE_IN_DQUOTE, STATE_IN_DQUOTE, STATE_IN_DQUOTE, STATE_IN_DQUOTE, STATE_IN_DQUOTE, STATE_IN_DQUOTE, STATE_ERROR}, // STATE_IN_DQUOTE
    {STATE_IN_SQUOTE, STATE_IN_SQUOTE, STATE_IN_SQUOTE, STATE_IN_SQUOTE, STATE_IN_SQUOTE, STATE_IN_SQUOTE, STATE_IN_SQUOTE, STATE_IN_SQUOTE, STATE_IN_SQUOTE, STATE_IN_SQUOTE, STATE_ERROR}, // STATE_IN_SQUOTE
    {STATE_WORD, STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_IN_PAREN, STATE_ERROR, STATE_ERROR}, // STATE_AFTER_OP
    {STATE_WORD, STATE_AFTER_OP, STATE_AFTER_OP, STATE_AFTER_OP, STATE_AFTER_OP, STATE_AFTER_OP, STATE_AFTER_OP, STATE_AFTER_OP, STATE_IN_PAREN, STATE_ERROR, STATE_ACCEPT}, // STATE_WORD
    {STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR}  // STATE_ACCEPT
};    
    static int finals[MAX_STATES] = {0};
    finals[STATE_WORD] = 1;
    finals[STATE_ACCEPT] = 1;
    
    DFA_lexer.transition_table = transition_matrix;
    DFA_lexer.initial_state = STATE_INITIAL;
    DFA_lexer.current_state = STATE_INITIAL;
    DFA_lexer.final_states = finals;
    DFA_lexer.context = (void*)tokens;
    
    return DFA_lexer;
}

void evaluate(t_token *tokens) {
    DFA DFA_lexer = set_lexer_DFA(tokens);
    t_token *current = tokens;
    
    while (current) {
        DFA_lexer.current_state = 
            DFA_lexer.transition_table[DFA_lexer.current_state][current->type];
        
        if (DFA_lexer.current_state == STATE_ERROR) {
            ft_printf("Error de sintaxis cerca de '%s'\n", current->value);
            return;
        }
        
        current = current->next;
    }
    
    if (DFA_lexer.final_states[DFA_lexer.current_state]) {
        return;
    } else {
        ft_printf("Error: comando incompleto\n");
    }
}

/*
 * las redirecciones pueden estar juntas o separada
 * "<infile" o "< infile" asi que ese strncmp esta mal
 *
 */
t_token_type set_token_type(char *tok) {
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
    return TOKEN_EOF;
  else
    return TOKEN_WORD;
}

t_token* set_token(char *tok){

    t_token *new_node;

    new_node = (t_token *)malloc(sizeof(t_token));
    if (!new_node)
      return NULL;
    new_node->value = tok;
    new_node->type = set_token_type(tok);
    new_node->next = NULL;
    new_node->prev = NULL;
    return new_node;
}

t_token* lexer(char *input) {
  t_token *tokens = NULL;
  t_token *current = NULL;
  char *tok;
  
    tok = ft_strtok(input, " ");
  while (tok) {
    t_token *new_node = set_token(tok);
    if (!new_node)
      return NULL;
      
    if (!tokens)
      tokens = new_node;
    else
    {
      current->next = new_node;
      new_node->prev = current;
    }
    
    current = new_node;
    tok = ft_strtok(NULL, " ");
  }
  ft_print_token_list(tokens);
  evaluate(tokens);
  return tokens;
}
