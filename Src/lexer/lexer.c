#include "../libft/libft.h"
#include "../../Inc/minishell.h"
#include <readline/readline.h>
#include <readline/history.h>

// ========== DELIMITER FUNCTIONS ==========

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

char **get_delimiters(void)
{
    static char *delims[] = {"<<", ">>", "&&", "||", " ", "\t", 
                          "<", ">", "(", ")", "\"", "'", "|", ";", "&", NULL};
    return delims;
}

// ========== TOKEN TYPE FUNCTIONS ==========

t_token_type get_redirection_type(char *value)
{
    if (!value)
        return TOKEN_WORD;
    if (ft_strncmp(value, "<<", 3) == 0)
        return TOKEN_HEREDOC;
    else if (ft_strncmp(value, ">>", 3) == 0)
        return TOKEN_APPEND;
    else if (ft_strncmp(value, "<", 2) == 0)
        return TOKEN_REDIR_IN;
    else if (ft_strncmp(value, ">", 2) == 0)
        return TOKEN_REDIR_OUT;
    return TOKEN_WORD;
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

t_token_type get_quote_type(char quote_char)
{
    if (quote_char == '\'')
        return TOKEN_LITERAL_WORD;
    return TOKEN_WORD;
}

int is_redirection(char *value)
{
    return (ft_strncmp(value, "<", 1) == 0 || 
            ft_strncmp(value, ">", 1) == 0);
}

// ========== TOKEN CREATION AND MANAGEMENT ==========

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

void free_token_list(t_token *head)
{
    t_token *temp;
    
    while (head)
    {
        temp = head;
        head = head->next;
        free(temp->value);
        free(temp);
    }
}

// ========== UTILITY FUNCTIONS ==========

char *skip_spaces(char *str)
{
    while (str && *str && (*str == ' ' || *str == '\t'))
        str++;
    return str;
}

char *concatenate_input(char *original, char *continuation)
{
    char *new_input;
    char *with_newline;
    
    if (!original)
        return ft_strdup(continuation);
    with_newline = ft_strjoin(original, "\n");
    if (!with_newline)
        return NULL;
    new_input = ft_strjoin(with_newline, continuation);
    free(with_newline);
    free(continuation);
    return new_input;
}

// ========== INPUT VALIDATION ==========

int check_unclosed_quotes(char *str, char *quote_pos)
{
    (void)str;
    char quote_char;
    char *end;
    
    quote_char = *quote_pos;
    end = quote_pos + 1;
    while (*end)
    {
        if (*end == quote_char)
            return 0;
        end++;
    }
    return 1;
}

int check_unclosed_parens(char *str)
{
    int level;
    
    level = 0;
    while (*str)
    {
        if (*str == '(')
            level++;
        else if (*str == ')')
            level--;
        str++;
    }
    return level;
}

char *find_first_quote(char *str)
{
    while (*str)
    {
        if (*str == '"' || *str == '\'')
            return str;
        str++;
    }
    return NULL;
}

char *handle_unclosed_quotes(char *input)
{
    char *continuation;
    char *new_input;
    
    continuation = readline("> ");
    if (!continuation)
    {
        ft_printf("syntax error: unterminated quoted string\n");
        return NULL;
    }
    new_input = concatenate_input(input, continuation);
    return new_input;
}

char *handle_unclosed_parens(char *input)
{
    char *continuation;
    char *new_input;
    
    continuation = readline("> ");
    if (!continuation)
    {
        ft_printf("syntax error: unexpected end of file\n");
        return NULL;
    }
    new_input = concatenate_input(input, continuation);
    return new_input;
}

char *preprocess_input(char *input)
{
    char *current_input;
    char *quote_pos;
    int paren_level;
    
    current_input = ft_strdup(input);
    if (!current_input)
        return NULL;
    while (1)
    {
        quote_pos = find_first_quote(current_input);
        if (quote_pos && check_unclosed_quotes(current_input, quote_pos))
        {
            current_input = handle_unclosed_quotes(current_input);
            if (!current_input)
                return NULL;
            continue;
        }
        paren_level = check_unclosed_parens(current_input);
        if (paren_level > 0)
        {
            current_input = handle_unclosed_parens(current_input);
            if (!current_input)
                return NULL;
            continue;
        }
        break;
    }
    return current_input;
}

// ========== QUOTE PROCESSING ==========

char *find_quote_end(char *start, char quote_char)
{
    char *pos;
    
    pos = start + 1;
    while (*pos)
    {
        if (*pos == quote_char)
            return pos;
        pos++;
    }
    return NULL;
}

char *process_quoted_text(t_token **head, t_token **current, 
                        char *input, t_token_type *next_type)
{
    char *closing_quote;
    char *content;
    t_token_type type;
    
    closing_quote = find_quote_end(input, *input);
    if (!closing_quote)
        return input + ft_strlen(input);
    content = ft_substr(input + 1, 0, closing_quote - (input + 1));
    type = get_quote_type(*input);
    add_token(head, current, content, type);
    *next_type = TOKEN_WORD;
    return closing_quote + 1;
}

// ========== PARENTHESIS PROCESSING ==========

char *find_closing_paren(char *start)
{
    int level;
    char *pos;
    
    level = 1;
    pos = start + 1;
    while (*pos)
    {
        if (*pos == '(')
            level++;
        else if (*pos == ')')
        {
            level--;
            if (level == 0)
                return pos;
        }
        pos++;
    }
    return NULL;
}

char *process_parentheses(t_token **head, t_token **current, 
                       char *input, t_token_type *next_type)
{
    char *closing_paren;
    char *content;
    
    closing_paren = find_closing_paren(input);
    if (!closing_paren)
        return input + ft_strlen(input);
    content = ft_substr(input + 1, 0, closing_paren - (input + 1));
    add_token(head, current, content, TOKEN_PAREN);
    *next_type = TOKEN_WORD;
    return closing_paren + 1;
}

// ========== WORD AND DELIMITER PROCESSING ==========

void process_word(t_token **head, t_token **current, 
               char *start, char *end, t_token_type *next_type)
{
    char *word;
    t_token_type current_type;
    
    if (end <= start)
        return;
    word = ft_substr(start, 0, end - start);
    current_type = *next_type;
    if (word && *word)
    {
        add_token(head, current, word, current_type);
        if (current_type != TOKEN_WORD && current_type != TOKEN_LITERAL_WORD)
            *next_type = TOKEN_WORD;
    }
    else if (word)
        free(word);
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
            *next_type = TOKEN_WORD;
        }
    }
}

void process_remaining(t_token **head, t_token **current, 
                     char *input, t_token_type *next_type)
{
    char *word;
    t_token_type current_type;
    
    if (!input || !*input)
        return;
    word = ft_strdup(input);
    current_type = *next_type;
    if (word && *word)
    {
        add_token(head, current, word, current_type);
        if (current_type != TOKEN_WORD && current_type != TOKEN_LITERAL_WORD)
            *next_type = TOKEN_WORD;
    }
    else if (word)
        free(word);
}

// ========== MAIN LEXER FUNCTIONS ==========

void handle_special_char(t_token **head, t_token **current, 
                       char **input, char **word_start, t_token_type *next_type)
{
    if (**input == '"' || **input == '\'')
    {
        process_word(head, current, *word_start, *input, next_type);
        *input = process_quoted_text(head, current, *input, next_type);
        *word_start = *input;
    }
    else if (**input == '(')
    {
        process_word(head, current, *word_start, *input, next_type);
        *input = process_parentheses(head, current, *input, next_type);
        *word_start = *input;
    }
    else if (**input == ')')
    {
        (*input)++;
        *word_start = *input;
    }
}

int process_tokens(t_token **head, t_token **current, char *input)
{
    t_token_type next_type;
    char *word_start;
    char *tok;
    char *delimiter;
    
    next_type = TOKEN_WORD;
    input = skip_spaces(input);
    word_start = input;
    while (*input)
    {
        if (*input == '"' || *input == '\'' || *input == '(' || *input == ')')
        {
            handle_special_char(head, current, &input, &word_start, &next_type);
            continue;
        }
        tok = ft_strarraystr(input, get_delimiters());
        if (!tok)
        {
            process_remaining(head, current, word_start, &next_type);
            break;
        }
        process_word(head, current, word_start, tok, &next_type);
        delimiter = identify_delimiter(tok, get_delimiters());
        process_delimiter(head, current, delimiter, &next_type);
        input = tok + (delimiter ? ft_strlen(delimiter) : 1);
        input = skip_spaces(input);
        word_start = input;
    }
    return 1;
}

t_token *lexer(char *input)
{
    t_token *head;
    t_token *current;
    char *processed_input;
    
    if (!input)
        return NULL;
    processed_input = preprocess_input(input);
    if (!processed_input)
        return NULL;
    head = NULL;
    current = NULL;
    process_tokens(&head, &current, processed_input);
    free(processed_input);
    return head;
}
