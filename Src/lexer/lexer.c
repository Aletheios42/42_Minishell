#include "../libft/libft.h"
#include "../../Inc/minishell.h"
#include <readline/readline.h>
#include <readline/history.h>

// ========== METACHARACTER CLASSIFICATION ==========

int is_ifs_char(char c)
{
    return (c == ' ' || c == '\t' || c == '\n');
}

int is_logical_operator(char *str)
{
    return (ft_strncmp(str, "&&", 2) == 0 || 
            ft_strncmp(str, "||", 2) == 0 || 
            ft_strncmp(str, "|", 1) == 0 ||
            ft_strncmp(str, ";", 1) == 0);
}

int is_redirection_operator(char *str)
{
    return (ft_strncmp(str, "<<", 2) == 0 ||
            ft_strncmp(str, ">>", 2) == 0 ||
            ft_strncmp(str, "<", 1) == 0 ||
            ft_strncmp(str, ">", 1) == 0);
}

int is_parenthesis_operator(char *str)
{
    return (*str == '(' || *str == ')');
}

int is_quote_char(char c)
{
    return (c == '"' || c == '\'');
}

char *get_operator_at_position(char *str)
{
    if (ft_strncmp(str, "&&", 2) == 0) return "&&";
    if (ft_strncmp(str, "||", 2) == 0) return "||";
    if (ft_strncmp(str, "<<", 2) == 0) return "<<";
    if (ft_strncmp(str, ">>", 2) == 0) return ">>";
    if (ft_strncmp(str, "|", 1) == 0) return "|";
    if (ft_strncmp(str, ";", 1) == 0) return ";";
    if (ft_strncmp(str, "<", 1) == 0) return "<";
    if (ft_strncmp(str, ">", 1) == 0) return ">";
    if (*str == '(') return "(";
    if (*str == ')') return ")";
    return NULL;
}

t_token_type get_operator_token_type(char *op)
{
    if (ft_strcmp(op, "&&") == 0) return TOKEN_AND;
    if (ft_strcmp(op, "||") == 0) return TOKEN_OR;
    if (ft_strcmp(op, "|") == 0) return TOKEN_PIPE;
    if (ft_strcmp(op, "<<") == 0) return TOKEN_HEREDOC;
    if (ft_strcmp(op, ">>") == 0) return TOKEN_APPEND;
    if (ft_strcmp(op, "<") == 0) return TOKEN_REDIR_IN;
    if (ft_strcmp(op, ">") == 0) return TOKEN_REDIR_OUT;
    if (ft_strcmp(op, "(") == 0) return TOKEN_PAREN_OPEN;
    if (ft_strcmp(op, ")") == 0) return TOKEN_PAREN_CLOSE;
    return TOKEN_WORD;
}

// ========== QUOTE PROCESSING ==========

typedef struct s_quote_context {
    char quote_char;
    int in_quotes;
    t_token_type quote_type;
} t_quote_context;

void init_quote_context(t_quote_context *ctx)
{
    ctx->quote_char = 0;
    ctx->in_quotes = 0;
    ctx->quote_type = TOKEN_WORD;
}

int process_quote_char(t_quote_context *ctx, char c)
{
    if (!ctx->in_quotes && is_quote_char(c))
    {
        ctx->in_quotes = 1;
        ctx->quote_char = c;
        ctx->quote_type = (c == '\'') ? TOKEN_LITERAL_WORD : TOKEN_WORD;
        return 1;
    }
    else if (ctx->in_quotes && c == ctx->quote_char)
    {
        ctx->in_quotes = 0;
        ctx->quote_char = 0;
        return 1;
    }
    return 0;
}

// ========== TOKEN MANAGEMENT ==========

t_token *create_token(char *value, t_token_type type)
{
    t_token *token;
    
    if (!value)
        return NULL;
    token = malloc(sizeof(t_token));
    if (!token)
        return NULL;
    token->value = value;
    token->type = type;
    token->next = NULL;
    token->prev = NULL;
    return token;
}

void add_token_to_list(t_token **head, t_token **tail, t_token *new_token)
{
    if (!new_token)
        return;
    if (!*head)
    {
        *head = *tail = new_token;
        return;
    }
    (*tail)->next = new_token;
    new_token->prev = *tail;
    *tail = new_token;
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

// ========== STRING BUILDER ==========

typedef struct s_string_builder {
    char *str;
    size_t len;
    size_t capacity;
} t_string_builder;

t_string_builder *create_string_builder(void)
{
    t_string_builder *sb;
    
    sb = malloc(sizeof(t_string_builder));
    if (!sb)
        return NULL;
    sb->capacity = 64;
    sb->str = malloc(sb->capacity);
    if (!sb->str)
    {
        free(sb);
        return NULL;
    }
    sb->len = 0;
    sb->str[0] = '\0';
    return sb;
}

int append_char_to_builder(t_string_builder *sb, char c)
{
    char *new_str;
    
    if (sb->len + 1 >= sb->capacity)
    {
        sb->capacity *= 2;
        new_str = ft_realloc(sb->str, sizeof(char), sb->capacity);
        if (!new_str)
            return 0;
        sb->str = new_str;
    }
    sb->str[sb->len++] = c;
    sb->str[sb->len] = '\0';
    return 1;
}

char *finalize_string_builder(t_string_builder *sb)
{
    char *result;
    
    if (!sb)
        return NULL;
    if (sb->len == 0)
    {
        free(sb->str);
        free(sb);
        return ft_strdup("");
    }
    result = ft_strdup(sb->str);
    free(sb->str);
    free(sb);
    return result;
}

// ========== INPUT VALIDATION ==========

char *skip_whitespace(char *str)
{
    while (str && *str && is_ifs_char(*str))
        str++;
    return str;
}

int check_unclosed_quotes(char *input)
{
    t_quote_context ctx;
    
    init_quote_context(&ctx);
    while (*input)
    {
        process_quote_char(&ctx, *input);
        input++;
    }
    return ctx.in_quotes;
}

int check_unclosed_parens(char *input)
{
    int level = 0;
    t_quote_context ctx;
    
    init_quote_context(&ctx);
    while (*input)
    {
        process_quote_char(&ctx, *input);
        if (!ctx.in_quotes)
        {
            if (*input == '(') level++;
            else if (*input == ')') level--;
        }
        input++;
    }
    return level;
}

int check_syntax_errors(char *input)
{
    char *pos = input;
    
    while (*pos)
    {
        if (ft_strncmp(pos, "|||", 3) == 0)
        {
            ft_printf("syntax error near unexpected token `|'\n");
            return 1;
        }
        if (ft_strncmp(pos, "&&&", 3) == 0)
        {
            ft_printf("syntax error near unexpected token `&'\n");
            return 1;
        }
        pos++;
    }
    return 0;
}

char *handle_multiline_input(char *input)
{
    char *line;
    char *new_input;
    char *temp;
    
    if (check_syntax_errors(input))
    {
        free(input);
        return NULL;
    }
    
    while (check_unclosed_quotes(input) || check_unclosed_parens(input) > 0)
    {
        if (check_unclosed_quotes(input))
            line = readline("quote> ");
        else
            line = readline("paren> ");
            
        if (!line)
        {
            if (check_unclosed_quotes(input))
                ft_printf("syntax error: unterminated quoted string\n");
            else
                ft_printf("syntax error: unexpected end of file\n");
            free(input);
            return NULL;
        }
        temp = ft_strjoin(input, "\n");
        free(input);
        new_input = ft_strjoin(temp, line);
        free(temp);
        free(line);
        input = new_input;
    }
    return input;
}

// ========== MAIN TOKENIZER ==========

int add_current_token(t_string_builder *sb, t_token **head, 
                     t_token **tail, t_token_type token_type)
{
    char *token_value;
    t_token *new_token;
    
    if (!sb || sb->len == 0)
    {
        if (sb)
        {
            if (sb->str)
                free(sb->str);
            free(sb);
        }
        return 1;
    }
    
    token_value = finalize_string_builder(sb);
    if (!token_value)
        return 0;
     
    new_token = create_token(token_value, token_type);
    if (!new_token)
    {
        free(token_value);
        return 0;
    }
    
    add_token_to_list(head, tail, new_token);
    return 1;
}

int add_operator_token(char *op, t_token **head, t_token **tail)
{
    t_token *new_token;
    char *op_value;
    
    op_value = ft_strdup(op);
    if (!op_value)
        return 0;
    new_token = create_token(op_value, get_operator_token_type(op));
    if (!new_token)
    {
        free(op_value);
        return 0;
    }
    add_token_to_list(head, tail, new_token);
    return 1;
}

t_token *tokenize_input(char *input)
{
    t_token *head = NULL;
    t_token *tail = NULL;
    t_string_builder *current_token;
    t_quote_context quote_ctx;
    char *operator;
    t_token_type token_type;
    
    current_token = create_string_builder();
    if (!current_token)
        return NULL;
    init_quote_context(&quote_ctx);
    token_type = TOKEN_WORD;


    input = skip_whitespace(input);
    while (*input)
    {
        // Process quotes (but don't add them to the token)
        if (process_quote_char(&quote_ctx, *input))
        {

            if (!quote_ctx.in_quotes)
                token_type = quote_ctx.quote_type;
            input++;
            continue;
        }
        
        // Only process separators if NOT in quotes
        if (!quote_ctx.in_quotes)
        {
            // Handle whitespace separators
            if (is_ifs_char(*input))
            {
                if (!add_current_token(current_token, &head, &tail, token_type))
                {
                    free_token_list(head);
                    return NULL;
                }
                current_token = create_string_builder();
                if (!current_token)
                {
                    free_token_list(head);
                    return NULL;
                }
                token_type = TOKEN_WORD;
                input = skip_whitespace(input);
                continue;
            }
            
            // Handle operator separators
            operator = get_operator_at_position(input);
            if (operator)
            {
                if (!add_current_token(current_token, &head, &tail, token_type))
                {
                    free_token_list(head);
                    return NULL;
                }
                if (!add_operator_token(operator, &head, &tail))
                {
                    free_token_list(head);
                    return NULL;
                }
                current_token = create_string_builder();
                if (!current_token)
                {
                    free_token_list(head);
                    return NULL;
                }
                token_type = TOKEN_WORD;
                input += ft_strlen(operator);
                input = skip_whitespace(input);
                continue;
            }
        }
        
        // Add character to current token
        if (!append_char_to_builder(current_token, *input))
        {
            if (current_token)
            {
                if (current_token->str)
                    free(current_token->str);
                free(current_token);
            }
            free_token_list(head);
            return NULL;
        }
        input++;
    }
    
    // Add final token
    if (!add_current_token(current_token, &head, &tail, token_type))
    {
        free_token_list(head);
        return NULL;
    }
    
    return head;
}

// ========== PUBLIC INTERFACE ==========

t_token *lexer(char *input)
{
    char *processed_input;
    t_token *tokens;
    
    if (!input)
        return NULL;
    
    processed_input = ft_strdup(input);
    if (!processed_input)
        return NULL;
    
    processed_input = handle_multiline_input(processed_input);
    if (!processed_input)
        return NULL;
    
    tokens = tokenize_input(processed_input);
    free(processed_input);
    
    return tokens;
}
