
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alepinto <alepinto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 15:00:00 by alepinto          #+#    #+#             */
/*   Updated: 2025/05/23 15:00:00 by alepinto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Inc/minishell.h"
#include "../../libft/libft.h"

// Forward declarations
static char *get_variable_name(char *str, int *len);
static char *expand_variable_with_locals(char *var_name, t_env *env, t_local_vars *locals, int exit_status);
static char *expand_string_with_locals(char *str, t_env *env, t_local_vars *locals, int exit_status);


static char *get_variable_name(char *str, int *len)
{
    int i = 0;
    
    if (str[0] == '?')
    {
        *len = 1;
        return ft_strdup("?");
    }
    if (str[0] == '{')
    {
        i = 1;
        while (str[i] && str[i] != '}')
            i++;
        if (str[i] == '}')
        {
            *len = i + 1;
            return ft_substr(str, 1, i - 1);
        }
    }
    while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
        i++;
    *len = i;
    return ft_substr(str, 0, i);
}

static char *expand_variable_with_locals(char *var_name, t_env *env, t_local_vars *locals, int exit_status)
{
    char *value;
    
    if (ft_strcmp(var_name, "?") == 0)
        return ft_itoa(exit_status);
    
    value = local_var_get(locals, var_name);
    if (value)
        return ft_strdup(value);
    
    value = get_env_value(env, var_name);
    if (value)
        return ft_strdup(value);
    
    return ft_strdup("");
}

static char *expand_string_with_locals(char *str, t_env *env, t_local_vars *locals, int exit_status)
{
    char *result = ft_strdup("");
    char *temp;
    char *var_name;
    char *var_value;
    char single_char[2];
    int i = 0;
    int len;
    
    while (str[i])
    {
        if (str[i] == '$' && str[i + 1])
        {
            var_name = get_variable_name(&str[i + 1], &len);
            var_value = expand_variable_with_locals(var_name, env, locals, exit_status);
            temp = ft_strjoin(result, var_value);
            free(result);
            free(var_name);
            free(var_value);
            result = temp;
            i += len + 1;
        }
        else
        {
            single_char[0] = str[i];
            single_char[1] = '\0';
            temp = ft_strjoin(result, single_char);
            free(result);
            result = temp;
            i++;
        }
    }
    return result;
}

void expand_token_list(t_token *tokens, t_env *env, t_local_vars *locals, int exit_status)
{
    char *expanded;
    
    while (tokens)
    {
        if (tokens->type == TOKEN_WORD)
        {
            if (locals)
                expanded = expand_string_with_locals(tokens->value, env, locals, exit_status);
            else
                expanded = expand_string_with_locals(tokens->value, env, NULL, exit_status);
            free(tokens->value);
            tokens->value = expanded;
        }
        tokens = tokens->next;
    }
}

void expand_tokens(t_token *tokens, t_env *env, int exit_status)
{
    expand_token_list(tokens, env, NULL, exit_status);
}
