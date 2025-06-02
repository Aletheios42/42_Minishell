/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_v4.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elorente <elorente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 18:05:02 by alepinto          #+#    #+#             */
/*   Updated: 2025/05/27 18:06:51 by elorente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Inc/minishell.h"
#include "../libft/libft.h"
#include <dirent.h>
#include <sys/stat.h>

char **split_expanded_value(const char *expanded_value)
{
    char **words;
    int i;
    
    if (!expanded_value)
        return NULL;
    words = ft_split(expanded_value, ' ');
    if (!words)
        return NULL;
    i = 0;
    while (words[i])
    {
        if (words[i][0] == '\0')
        {
            free(words[i]);
            int j = i;
            while (words[j])
            {
                words[j] = words[j + 1];
                j++;
            }
        }
        else
        {
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
        if (words[i][0] != '\0')
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

// ========== TOKEN EXPANSION WITH WILDCARD ==========

t_token *expand_and_split_token_copy(t_token *original, t_env *env, int exit_status)
{
    char *expanded_value;
    char **wildcard_matches;
    char **split_words;
    t_token *new_tokens;
    
    if (!original)
        return NULL;
    if (original->type == TOKEN_LITERAL_WORD)
    {
        t_token *copy = create_token(ft_strdup(original->value), original->type);
        return copy;
    }
    if (ft_strchr(original->value, '='))
    {
        char *expanded_value = expand_string(original->value, env, exit_status);
        t_token *new_token = create_token(expanded_value, original->type);
        return new_token;
    }
    expanded_value = expand_string(original->value, env, exit_status);
    if (!expanded_value)
        return NULL;
    if (has_wildcard(expanded_value))
    {
        wildcard_matches = expand_wildcard_pattern(expanded_value);
        if (wildcard_matches)
        {
            new_tokens = create_tokens_from_words(wildcard_matches, original->type);
            ft_free_matrix(wildcard_matches);
            free(expanded_value);
            return new_tokens;
        }
    }
    if (!needs_splitting(expanded_value))
    {
        t_token *new_token = create_token(expanded_value, original->type);
        return new_token;
    }
    split_words = split_expanded_value(expanded_value);
    free(expanded_value);
    if (!split_words)
        return NULL;
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
        expanded_tokens = expand_and_split_token_copy(current_original, env, exit_status);
        if (expanded_tokens)
        {
            if (!expanded_head)
            {
                expanded_head = expanded_tokens;
                expanded_tail = expanded_tokens;
                while (expanded_tail->next)
                    expanded_tail = expanded_tail->next;
            }
            else
            {
                expanded_tail->next = expanded_tokens;
                expanded_tokens->prev = expanded_tail;
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
    expanded_value = expand_string(value, *env, 0);
    env_set_value(env, key, expanded_value);
    free(key);
    free(value);
    free(expanded_value);
    return 1;
}
