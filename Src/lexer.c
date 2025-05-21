#include "../libft/libft.h"
#include "../Inc/minishell.h"

char *identify_delimiter(char *str, char **delims)
{
    int i;
    size_t len;
    char *match;
    
    i = 0;
    len = 0;
    match = NULL;
    while (delims[i])
    {
        if (ft_strncmp(str, delims[i], ft_strlen(delims[i])) == 0)
        {
            if (ft_strlen(delims[i]) > len)
            {
                len = ft_strlen(delims[i]);
                match = delims[i];
            }
        }
        i++;
    }
    return match;
}

t_token_type get_redirection_type(char *value)
{
    if (!value)
        return TOKEN_WORD;
    if (ft_strncmp(value, "<", 2) == 0)
        return TOKEN_REDIR_IN;
    else if (ft_strncmp(value, ">", 2) == 0)
        return TOKEN_REDIR_OUT;
    else if (ft_strncmp(value, ">>", 3) == 0)
        return TOKEN_APPEND;
    else if (ft_strncmp(value, "<<", 3) == 0)
        return TOKEN_HEREDOC;
    return TOKEN_WORD;
}

int is_redirection(char *value)
{
    return (ft_strncmp(value, "<", 1) == 0 || 
            ft_strncmp(value, ">", 1) == 0);
}

t_token *create_token(char *value, t_token_type type)
{
    t_token *node;
    
    if (!value)
        return NULL;
    node = (t_token *)malloc(sizeof(t_token));
    if (!node)
        return NULL;
    node->value = value;
    node->type = type;
    node->next = NULL;
    node->prev = NULL;
    return node;
}

void add_token(t_token **head, t_token **current, char *value, t_token_type type)
{
    t_token *new_token;

    if (!value || !*value)
    {
        free(value);
        return;
    }
    new_token = create_token(value, type);
    if (!new_token)
        return;
    if (!*head)
    {
        *head = new_token;
        *current = new_token;
        return;
    }
    (*current)->next = new_token;
    new_token->prev = *current;
    *current = new_token;
}

char *skip_spaces(char *str)
{
    while (str && *str && (*str == ' ' || *str == '\t'))
        str++;
    return str;
}

t_token_type get_quote_type(char quote_char)
{
    if (quote_char == '\'')
        return TOKEN_LITERAL_WORD;
    return TOKEN_WORD;
}

char *process_quoted_text(t_token **head, t_token **current, 
                        char *input, t_token_type *next_type)
{
    char *closing_quote;
    char *content;
    t_token_type type;
    
    closing_quote = ft_strchr(input + 1, *input);
    if (!closing_quote)
        return input + ft_strlen(input);
    
    content = ft_substr(input + 1, 0, closing_quote - (input + 1));
    type = get_quote_type(*input);
    
    add_token(head, current, content, type);
    *next_type = TOKEN_WORD;
    return closing_quote + 1;
}

char *find_closing_paren(char *input)
{
    int level;
    char *pos;
    
    level = 1;
    pos = input + 1;
    while (*pos && level > 0)
    {
        if (*pos == '(')
            level++;
        else if (*pos == ')')
            level--;
        if (level > 0)
            pos++;
    }
    return pos;
}

char *process_parentheses(t_token **head, t_token **current, 
                       char *input, t_token_type *next_type)
{
    char *closing_paren;
    char *content;
    
    closing_paren = find_closing_paren(input);
    if (!closing_paren || *closing_paren != ')')
        return input + ft_strlen(input);
    
    content = ft_substr(input + 1, 0, closing_paren - (input + 1));
    add_token(head, current, content, TOKEN_PAREN);
    
    *next_type = TOKEN_WORD;
    return closing_paren + 1;
}

void process_word(t_token **head, t_token **current, 
               char *start, char *end, t_token_type type)
{
    char *word;
    
    if (end <= start)
        return;
    
    word = ft_substr(start, 0, end - start);
    if (word && *word)
        add_token(head, current, word, type);
    else if (word)
        free(word);
}

t_token_type get_operator_type(char *op)
{
    if (ft_strncmp(op, "&&", 2) == 0)
        return TOKEN_AND;
    else if (ft_strncmp(op, "||", 2) == 0)
        return TOKEN_OR;
    else if (ft_strncmp(op, "|", 1) == 0)
        return TOKEN_PIPE;
    return TOKEN_WORD;
}

void process_delimiter(t_token **head, t_token **current, 
                    char *delimiter, t_token_type *next_type)
{
    char *op_token;
    t_token_type op_type;
    
    if (is_redirection(delimiter))
        *next_type = get_redirection_type(delimiter);
    else if (*delimiter != ' ' && *delimiter != '\t' && 
             *delimiter != '"' && *delimiter != '\'' && 
             *delimiter != '(' && *delimiter != ')')
    {
        op_token = ft_strdup(delimiter);
        if (op_token)
        {
            op_type = get_operator_type(op_token);
            add_token(head, current, op_token, op_type);
            *next_type = TOKEN_WORD; // Reset after creating operator token
        }
    }
}


char **get_delimiters(void)
{
    static char *delims[] = {" ", "\t", "<<", ">>", "<", ">", "(", ")", 
                          "\"", "'", "&&", "|", "||", ";", "&", NULL};
    return delims;
}

void handle_special_char(t_token **head, t_token **current, 
                       char **input, char **word_start, t_token_type *next_type)
{
    if (**input == '"' || **input == '\'')
    {
        process_word(head, current, *word_start, *input, *next_type);
        *input = process_quoted_text(head, current, *input, next_type);
        *word_start = *input;
    }
    else if (**input == '(')
    {
        process_word(head, current, *word_start, *input, *next_type);
        *input = process_parentheses(head, current, *input, next_type);
        *word_start = *input;
    }
    else if (**input == ')')
    {
        (*input)++;
        *word_start = *input;
    }
}

t_token *lexer(char *input)
{
    t_token *head;
    t_token *current;
    t_token_type next_type;
    char *word_start;
    char *tok;
    char *delimiter;
    
    if (!input)
        return NULL;
    
    head = NULL;
    current = NULL;
    next_type = TOKEN_WORD;
    input = skip_spaces(input);
    word_start = input;
    
    while (*input)
    {
        if (*input == '"' || *input == '\'' || *input == '(' || *input == ')')
        {
            handle_special_char(&head, &current, &input, &word_start, &next_type);
            continue;
        }
        
        tok = ft_strarraystr(input, get_delimiters());
        if (!tok)
        {
            add_token(&head, &current, ft_strdup(word_start), next_type);
            break;
        }
        
        process_word(&head, &current, word_start, tok, next_type);
        delimiter = identify_delimiter(tok, get_delimiters());
        process_delimiter(&head, &current, delimiter, &next_type);
        
        input = tok + (delimiter ? ft_strlen(delimiter) : 1);
        input = skip_spaces(input);
        word_start = input;
    }
    
    return head;
}
