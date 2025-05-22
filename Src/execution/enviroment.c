#include "../../Inc/minishell.h"
#include "../../libft/libft.h"

char *get_env_value(t_env *env, const char *key)
{
    while (env)
    {
        if (ft_strcmp(env->key, key) == 0)
        {
            return env->value;
        }
        env = env->next;
    }
    return NULL;
}

 t_env *create_env_node(char *env_str)
{
    t_env *node;
    char *equal_pos;

    node = malloc(sizeof(t_env));
    if (!node)
        return NULL;
    equal_pos = ft_strchr(env_str, '=');
    if (equal_pos)
    {
        node->key = ft_substr(env_str, 0, equal_pos - env_str);
        if (*(equal_pos + 1) == '\0')
            node->value = NULL;
        else
            node->value = ft_strdup(equal_pos + 1);
    }
    else
    {
        node->key = ft_strdup(env_str);
        node->value = NULL;
    }
    node->next = NULL;
    if (!node->key || (equal_pos && *(equal_pos + 1) != '\0' && !node->value))
    {
        free(node->key);
        free(node->value);
        free(node);
        return NULL;
    }
    return node;
}

 void free_env_list(t_env *head)
{
    t_env *current;

    while (head)
    {
        current = head->next;
        free(head->key);
        free(head->value);
        free(head);
        head = current;
    }
}

t_env *env_from_array(char **envp)
{
    t_env *head = NULL;
    t_env *current = NULL;
    t_env *new_node;
    int i = 0;

    if (!envp)
        return NULL;
    while (envp[i])
    {
        new_node = create_env_node(envp[i]);
        if (!new_node)
        {
            printf("ERROR: create_env_node failed for: %s\n", envp[i]);
            free_env_list(head);
            return NULL;
        }
        
        if (!head)
            head = current = new_node;
        else
            current = current->next = new_node;
        i++;
    }
    return head;
}
