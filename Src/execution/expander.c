#include "../../Inc/minishell.h"
#include "../../libft/libft.h"

// ========== VARIABLE NAME EXTRACTION ==========

char *extract_variable_name(const char *str, int *consumed_len)
{
    int i;
    
    if (!str || !consumed_len)
        return NULL;
    
    *consumed_len = 0;
    
    // Handle special case $?
    if (str[0] == '?')
    {
        *consumed_len = 1;
        return ft_strdup("?");
    }
    
    // Handle ${var} syntax
    if (str[0] == '{')
    {
        i = 1;
        while (str[i] && str[i] != '}')
            i++;
        
        if (str[i] == '}')
        {
            *consumed_len = i + 1;
            return ft_substr(str, 1, i - 1);
        }
        // Invalid ${} syntax, treat as literal
        return NULL;
    }
    
    // Handle regular $var syntax
    i = 0;
    while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
        i++;
    
    if (i == 0)
        return NULL;
    
    *consumed_len = i;
    return ft_substr(str, 0, i);
}

// ========== VARIABLE VALUE RESOLUTION ==========

char *resolve_variable_value(const char *var_name, t_env *env, int exit_status)
{
    char *value;
    
    if (!var_name)
        return ft_strdup("");
    
    // Handle exit status
    if (ft_strcmp(var_name, "?") == 0)
        return ft_itoa(exit_status);
    
    // Look up in environment
    value = get_env_value(env, var_name);
    if (value)
        return ft_strdup(value);
    
    // Variable not found, return empty string
    return ft_strdup("");
}

// ========== STRING EXPANSION ==========

char *expand_string(const char *str, t_env *env, int exit_status)
{
    char *result;
    char *temp;
    char *var_name;
    char *var_value;
    char single_char[2];
    int i;
    int consumed;
    
    if (!str)
        return ft_strdup("");
    
    result = ft_strdup("");
    if (!result)
        return NULL;
    
    i = 0;
    while (str[i])
    {
        if (str[i] == '$' && str[i + 1])
        {
            // Extract variable name
            var_name = extract_variable_name(&str[i + 1], &consumed);
            if (var_name)
            {
                // Resolve variable value
                var_value = resolve_variable_value(var_name, env, exit_status);
                if (var_value)
                {
                    temp = ft_strjoin(result, var_value);
                    free(result);
                    free(var_value);
                    result = temp;
                    if (!result)
                    {
                        free(var_name);
                        return NULL;
                    }
                }
                free(var_name);
                i += consumed + 1;
            }
            else
            {
                // Invalid variable syntax, treat $ as literal
                single_char[0] = str[i];
                single_char[1] = '\0';
                temp = ft_strjoin(result, single_char);
                free(result);
                result = temp;
                if (!result)
                    return NULL;
                i++;
            }
        }
        else
        {
            // Regular character
            single_char[0] = str[i];
            single_char[1] = '\0';
            temp = ft_strjoin(result, single_char);
            free(result);
            result = temp;
            if (!result)
                return NULL;
            i++;
        }
    }
    
    return result;
}

// ========== TOKEN SPLITTING ==========

int needs_splitting(const char *str)
{
    if (!str)
        return 0;
    
    while (*str)
    {
        if (*str == ' ' || *str == '\t')
            return 1;
        str++;
    }
    return 0;
}

char **split_expanded_value(const char *expanded_value)
{
    char **words;
    int i;
    
    if (!expanded_value)
        return NULL;
    
    // Split on spaces and tabs
    words = ft_split(expanded_value, ' ');
    if (!words)
        return NULL;
    
    // Filter out empty strings and handle tabs
    i = 0;
    while (words[i])
    {
        if (words[i][0] == '\0')
        {
            free(words[i]);
            // Shift remaining elements
            int j = i;
            while (words[j])
            {
                words[j] = words[j + 1];
                j++;
            }
        }
        else
        {
            // Replace tabs with spaces
            int j = 0;
            while (words[i][j])
            {
                if (words[i][j] == '\t')
                    words[i][j] = ' ';
                j++;
            }
            i++;
        }
    }
    
    return words;
}

t_token *create_tokens_from_words(char **words, t_token_type base_type)
{
    t_token *head;
    t_token *tail;
    t_token *new_token;
    int i;
    
    if (!words)
        return NULL;
    
    head = NULL;
    tail = NULL;
    i = 0;
    
    while (words[i])
    {
        if (words[i][0] != '\0')  // Skip empty strings
        {
            new_token = create_token(ft_strdup(words[i]), base_type);
            if (!new_token)
            {
                free_token_list(head);
                return NULL;
            }
            
            if (!head)
                head = tail = new_token;
            else
            {
                tail->next = new_token;
                new_token->prev = tail;
                tail = new_token;
            }
        }
        i++;
    }
    
    return head;
}

// ========== TOKEN EXPANSION - CREATES COPIES ==========

t_token *expand_and_split_token_copy(t_token *original, t_env *env, int exit_status)
{
    char *expanded_value;
    char **split_words;
    t_token *new_tokens;
    
    if (!original)
        return NULL;
    
    // Don't expand literal words (single quotes)
    if (original->type == TOKEN_LITERAL_WORD)
    {
        // Return a copy of the original token
        t_token *copy = create_token(ft_strdup(original->value), original->type);
        return copy;
    }
    
    // Expand the token value
    expanded_value = expand_string(original->value, env, exit_status);
    if (!expanded_value)
        return NULL;
    
    // Check if splitting is needed
    if (!needs_splitting(expanded_value))
    {
        // No splitting needed, create single token
        t_token *new_token = create_token(expanded_value, original->type);
        return new_token;
    }
    
    // Split the expanded value into multiple words
    split_words = split_expanded_value(expanded_value);
    free(expanded_value);
    
    if (!split_words)
        return NULL;
    
    // Create new tokens from split words
    new_tokens = create_tokens_from_words(split_words, original->type);
    ft_free_matrix(split_words);
    
    return new_tokens;
}

// ========== TOKEN LIST EXPANSION - CREATES NEW LIST ==========

t_token *expand_token_list_copy(t_token *original_tokens, t_env *env, int exit_status)
{
    t_token *expanded_head;
    t_token *expanded_tail;
    t_token *current_original;
    t_token *expanded_tokens;
    
    if (!original_tokens)
        return NULL;
    
    expanded_head = NULL;
    expanded_tail = NULL;
    current_original = original_tokens;
    
    while (current_original)
    {
        // Expand current token (may create multiple tokens)
        expanded_tokens = expand_and_split_token_copy(current_original, env, exit_status);
        
        if (expanded_tokens)
        {
            // Add expanded token(s) to result list
            if (!expanded_head)
            {
                expanded_head = expanded_tokens;
                expanded_tail = expanded_tokens;
                
                // Find the last token in the expanded chain
                while (expanded_tail->next)
                    expanded_tail = expanded_tail->next;
            }
            else
            {
                // Link to existing list
                expanded_tail->next = expanded_tokens;
                expanded_tokens->prev = expanded_tail;
                
                // Update tail to point to last token
                while (expanded_tail->next)
                    expanded_tail = expanded_tail->next;
            }
        }
        
        current_original = current_original->next;
    }
    
    return expanded_head;
}

// ========== ASSIGNMENT PROCESSING ==========

int process_assignment(const char *assignment, t_env **env)
{
    char *key;
    char *value;
    char *expanded_value;
    
    if (!assignment || !env || !is_assignment(assignment))
        return 0;
    
    if (!parse_assignment(assignment, &key, &value))
        return 0;
    
    // Expand the value part
    expanded_value = expand_string(value, *env, 0);
    
    // Set in environment
    env_set_value(env, key, expanded_value);
    
    free(key);
    free(value);
    free(expanded_value);
    
    return 1;
}

// ========== LEGACY FUNCTIONS (keep for compatibility) ==========

t_token *expand_token(t_token *token, t_env *env, int exit_status)
{
    char *expanded_value;
    char **split_words;
    t_token *new_tokens;
    
    if (!token)
        return NULL;
    
    // Don't expand literal words (single quotes)
    if (token->type == TOKEN_LITERAL_WORD)
        return token;
    
    // Expand the token value
    expanded_value = expand_string(token->value, env, exit_status);
    if (!expanded_value)
        return token;
    
    // Check if splitting is needed
    if (!needs_splitting(expanded_value))
    {
        // No splitting needed, just update the token value
        free(token->value);
        token->value = expanded_value;
        return token;
    }
    
    // Split the expanded value into multiple words
    split_words = split_expanded_value(expanded_value);
    free(expanded_value);
    
    if (!split_words)
        return token;
    
    // Create new tokens from split words
    new_tokens = create_tokens_from_words(split_words, token->type);
    ft_free_matrix(split_words);
    
    if (!new_tokens)
        return token;
    
    // Copy original token's linking info to first new token
    if (token->prev)
    {
        token->prev->next = new_tokens;
        new_tokens->prev = token->prev;
    }
    
    // Find last new token and link to original's next
    t_token *last_new = new_tokens;
    while (last_new->next)
        last_new = last_new->next;
    
    if (token->next)
    {
        last_new->next = token->next;
        token->next->prev = last_new;
    }
    
    // Free original token
    free(token->value);
    free(token);
    
    return new_tokens;
}

void expand_token_list(t_token **tokens, t_env *env, int exit_status)
{
    t_token *current;
    t_token *next;
    t_token *expanded;
    
    if (!tokens || !*tokens)
        return;
    
    current = *tokens;
    while (current)
    {
        next = current->next;
        
        expanded = expand_token(current, env, exit_status);
        
        // Update head pointer if first token was replaced
        if (current == *tokens && expanded != current)
            *tokens = expanded;
        
        // Move to next token (accounting for potential splits)
        if (expanded == current)
            current = next;
        else
        {
            // Token was split, find the last expanded token
            while (expanded && expanded->next && expanded->next != next)
                expanded = expanded->next;
            current = next;
        }
    }
}

int process_assignment_token(t_token *token, t_env **env)
{
    char *key;
    char *value;
    char *expanded_value;
    
    if (!token || !env || !is_assignment(token->value))
        return 0;
    
    if (!parse_assignment(token->value, &key, &value))
        return 0;
    
    // Expand the value part
    expanded_value = expand_string(value, *env, 0);
    
    // Set in environment
    env_set_value(env, key, expanded_value);
    
    free(key);
    free(value);
    free(expanded_value);
    
    return 1;
}

void expand_and_split_tokens(t_token **tokens, t_env *env, int exit_status)
{
    if (!tokens || !*tokens)
        return;
    
    expand_token_list(tokens, env, exit_status);
}

int handle_assignments_in_tokens(t_token *tokens, t_env **env)
{
    t_token *current;
    int assignment_count;
    
    if (!tokens || !env)
        return 0;
    
    assignment_count = 0;
    current = tokens;
    
    while (current)
    {
        if (process_assignment_token(current, env))
            assignment_count++;
        current = current->next;
    }
    
    return assignment_count;
}
