#include "../../Inc/minishell.h"
#include "../../libft/libft.h"

// ========== ENVIRONMENT VALUE ACCESS ==========

char *get_env_value(t_env *env, const char *key)
{
    if (!key)
        return NULL;
    
    while (env)
    {
        if (env->key && ft_strcmp(env->key, key) == 0)
            return env->value;
        env = env->next;
    }
    return NULL;
}

int env_key_exists(t_env *env, const char *key)
{
    if (!key)
        return 0;
    
    while (env)
    {
        if (env->key && ft_strcmp(env->key, key) == 0)
            return 1;
        env = env->next;
    }
    return 0;
}

// ========== ENVIRONMENT MODIFICATION ==========

void env_set_value(t_env **env, const char *key, const char *value)
{
    t_env *current;
    t_env *new_node;
    
    if (!env || !key)
        return;
    
    // Search for existing key
    current = *env;
    while (current)
    {
        if (current->key && ft_strcmp(current->key, key) == 0)
        {
            // Update existing value
            free(current->value);
            current->value = value ? ft_strdup(value) : NULL;
            return;
        }
        if (!current->next)
            break;
        current = current->next;
    }
    
    // Create new node
    new_node = malloc(sizeof(t_env));
    if (!new_node)
        return;
    
    new_node->key = ft_strdup(key);
    new_node->value = value ? ft_strdup(value) : NULL;
    new_node->next = NULL;
    
    if (!new_node->key || (value && !new_node->value))
    {
        free(new_node->key);
        free(new_node->value);
        free(new_node);
        return;
    }
    
    // Add to list
    if (!*env)
        *env = new_node;
    else
        current->next = new_node;
}

void env_unset_key(t_env **env, const char *key)
{
    t_env *current;
    t_env *prev;
    
    if (!env || !*env || !key)
        return;
    
    current = *env;
    prev = NULL;
    
    while (current)
    {
        if (current->key && ft_strcmp(current->key, key) == 0)
        {
            // Remove node
            if (prev)
                prev->next = current->next;
            else
                *env = current->next;
            
            free(current->key);
            free(current->value);
            free(current);
            return;
        }
        prev = current;
        current = current->next;
    }
}

// ========== ASSIGNMENT PARSING ==========

int parse_assignment(const char *assignment, char **key, char **value)
{
    char *equal_pos;
    
    if (!assignment || !key || !value)
        return 0;
    
    *key = NULL;
    *value = NULL;
    
    equal_pos = ft_strchr(assignment, '=');
    if (!equal_pos)
        return 0;
    
    // Extract key
    *key = ft_substr(assignment, 0, equal_pos - assignment);
    if (!*key)
        return 0;
    
    // Validate key format
    if (!is_valid_identifier(*key))
    {
        free(*key);
        *key = NULL;
        return 0;
    }
    
    // Extract value
    *value = ft_strdup(equal_pos + 1);
    if (!*value)
    {
        free(*key);
        *key = NULL;
        return 0;
    }
    
    return 1;
}

int is_valid_identifier(const char *str)
{
    int i;
    
    if (!str || !*str)
        return 0;
    
    // First character must be letter or underscore
    if (!ft_isalpha(str[0]) && str[0] != '_')
        return 0;
    
    // Rest must be alphanumeric or underscore
    i = 1;
    while (str[i])
    {
        if (!ft_isalnum(str[i]) && str[i] != '_')
            return 0;
        i++;
    }
    
    return 1;
}

int is_assignment(const char *str)
{
    char *key;
    char *value;
    int result;
    
    if (!str)
        return 0;
    
    result = parse_assignment(str, &key, &value);
    free(key);
    free(value);
    
    return result;
}

// ========== ENVIRONMENT INITIALIZATION ==========

t_env *create_env_node(const char *key, const char *value)
{
    t_env *node;
    
    node = malloc(sizeof(t_env));
    if (!node)
        return NULL;
    
    node->key = key ? ft_strdup(key) : NULL;
    node->value = value ? ft_strdup(value) : NULL;
    node->next = NULL;
    
    if ((key && !node->key) || (value && !node->value))
    {
        free(node->key);
        free(node->value);
        free(node);
        return NULL;
    }
    
    return node;
}

t_env *create_env_node_from_string(const char *env_str)
{
    char *key;
    char *value;
    t_env *node;
    
    if (!env_str)
        return NULL;
    
    if (!parse_assignment(env_str, &key, &value))
        return NULL;
    
    node = create_env_node(key, value);
    free(key);
    free(value);
    
    return node;
}

t_env *env_from_array(char **envp)
{
    t_env *head;
    t_env *current;
    t_env *new_node;
    int i;
    
    if (!envp)
        return NULL;
    
    head = NULL;
    current = NULL;
    i = 0;
    
    while (envp[i])
    {
        new_node = create_env_node_from_string(envp[i]);
        if (!new_node)
        {
            free_env_list(head);
            return NULL;
        }
        
        if (!head)
            head = current = new_node;
        else
        {
            current->next = new_node;
            current = new_node;
        }
        i++;
    }
    
    return head;
}

// ========== ENVIRONMENT CONVERSION ==========

int count_env_vars(t_env *env)
{
    int count;
    
    count = 0;
    while (env)
    {
        if (env->key)
            count++;
        env = env->next;
    }
    return count;
}

char **env_to_array(t_env *env)
{
    char **envp;
    char *env_string;
    char *temp;
    int count;
    int i;
    
    count = count_env_vars(env);
    envp = malloc(sizeof(char *) * (count + 1));
    if (!envp)
        return NULL;
    
    i = 0;
    while (env && i < count)
    {
        if (env->key)
        {
            if (env->value)
            {
                temp = ft_strjoin(env->key, "=");
                if (!temp)
                {
                    ft_free_matrix(envp);
                    return NULL;
                }
                env_string = ft_strjoin(temp, env->value);
                free(temp);
            }
            else
            {
                env_string = ft_strjoin(env->key, "=");
            }
            
            if (!env_string)
            {
                ft_free_matrix(envp);
                return NULL;
            }
            
            envp[i] = env_string;
            i++;
        }
        env = env->next;
    }
    
    envp[i] = NULL;
    return envp;
}

// ========== ENVIRONMENT CLEANUP ==========

void free_env_list(t_env *head)
{
    t_env *current;
    t_env *next;
    
    current = head;
    while (current)
    {
        next = current->next;
        free(current->key);
        free(current->value);
        free(current);
        current = next;
    }
}

// ========== ENVIRONMENT UTILITIES ==========

void print_env_list(t_env *env)
{
    while (env)
    {
        if (env->key)
        {
            ft_printf("%s", env->key);
            if (env->value)
                ft_printf("=%s", env->value);
            ft_printf("\n");
        }
        env = env->next;
    }
}

t_env *copy_env_list(t_env *original)
{
    t_env *head;
    t_env *current;
    t_env *new_node;
    
    if (!original)
        return NULL;
    
    head = NULL;
    current = NULL;
    
    while (original)
    {
        if (original->key)
        {
            new_node = create_env_node(original->key, original->value);
            if (!new_node)
            {
                free_env_list(head);
                return NULL;
            }
            
            if (!head)
                head = current = new_node;
            else
            {
                current->next = new_node;
                current = new_node;
            }
        }
        original = original->next;
    }
    
    return head;
}
