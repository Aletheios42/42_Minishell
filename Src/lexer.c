#include "../libft/libft.h"
#include "../Inc/minishell.h"

char *identify_delimiter(char *str, char **delims)
{
    int i = 0;
    size_t len = 0;
    char *match = NULL;
    
    while (delims[i])
    {
        size_t current_len = ft_strlen(delims[i]);
        if (ft_strncmp(str, delims[i], current_len) == 0)
        {
            if (current_len > len)
            {
                len = current_len;
                match = delims[i];
            }
        }
        i++;
    }
    return match;
}

t_token_type set_token_type(char *value)
{
    if (!value)
        return TOKEN_EOF;
    
    if (ft_strncmp(value, "<", 2) == 0)
        return TOKEN_REDIR_IN;
    else if (ft_strncmp(value, ">", 2) == 0)
        return TOKEN_REDIR_OUT;
    else if (ft_strncmp(value, ">>", 3) == 0)
        return TOKEN_APPEND;
    else if (ft_strncmp(value, "<<", 3) == 0)
        return TOKEN_HEREDOC;
    else if (ft_strncmp(value, "&&", 3) == 0)
        return TOKEN_AND;
    else if (ft_strncmp(value, "||", 3) == 0)
        return TOKEN_OR;
    else if (ft_strncmp(value, "|", 2) == 0)
        return TOKEN_PIPE;
    else if (ft_strncmp(value, "(", 2) == 0)
        return TOKEN_OPEN_PAREN;
    else if (ft_strncmp(value, ")", 2) == 0)
        return TOKEN_CLOSE_PAREN;
    else
        return TOKEN_WORD;
}

t_token *new_token(char *value)
{
    t_token *node = NULL;
    
    if (!value)
        return NULL;
        
    node = (t_token *)malloc(sizeof(t_token));
    if (!node)
        return NULL;
        
    node->value = value;
    node->type = set_token_type(value);
    node->next = NULL;
    node->prev = NULL;
    
    return node;
}

void add_token_back(t_token **tokens, t_token *new_token)
{
    t_token *current;
    
    if (!new_token)
        return;
        
    if (!*tokens)
    {
        *tokens = new_token;
        return;
    }
    
    current = *tokens;
    while (current->next)
        current = current->next;
        
    current->next = new_token;
    new_token->prev = current;
}

char *skip_spaces(char *str)
{
    while (str && *str && (*str == ' ' || *str == '\t'))
        str++;
    return str;
}

void process_text(t_token **tokens, char *input, char *tok) 
{
    char *word;
    size_t len;
    
    if (tok > input) 
    {
        len = tok - input;
        word = ft_substr(input, 0, len);
        if (word && *word)
            add_token_back(tokens, new_token(word));
        else if (word)
            free(word);
    }
}

// Helper function to process a delimiter
void process_delimiter(t_token **tokens, char *tok, char **delims) 
{
    char *delimiter;
    char *word;
    
    delimiter = identify_delimiter(tok, delims);
    if (delimiter && *delimiter != ' ' && *delimiter != '\t') 
    {
        word = ft_strdup(delimiter);
        if (word)
            add_token_back(tokens, new_token(word));
    }
}

// Helper function to process the rest of input
void process_remaining(t_token **tokens, char *input) 
{
    char *word;
    
    word = ft_strdup(input);
    if (word && *word)
        add_token_back(tokens, new_token(word));
    else if (word)
        free(word);
}

// Main lexer function
t_token* lexer(char *input)
{
    char *tok = NULL;
    t_token *tokens = NULL;
    char *delims[] = {" ", "\t", "<<", ">>", "<", ">", "(", ")", "\"", "'", "&&", "|", "||", ";", "&", NULL};
    char *delimiter = NULL;
    
    if (!input)
        return NULL;
    
    input = skip_spaces(input);
    while (input && *input)
    {
        tok = ft_strarraystr(input, delims);
        if (!tok) 
        {
            process_remaining(&tokens, input);
            break;
        }
        
        process_text(&tokens, input, tok);
        process_delimiter(&tokens, tok, delims);
        
        delimiter = identify_delimiter(tok, delims);
        if (delimiter)
            input =  tok + ft_strlen(delimiter);
        else
            input = tok + 1;
        input = skip_spaces(input);
    }
    
    print_token_list(tokens);
    return tokens;
}
