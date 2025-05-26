/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_v2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alepinto <alepinto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 19:53:34 by alepinto          #+#    #+#             */
/*   Updated: 2025/05/26 19:53:34 by alepinto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Inc/minishell.h"
#include "../libft/libft.h"
#include <dirent.h>
#include <sys/stat.h>

int	has_wildcard(const char *str)
{
	if (!str)
		return (0);
	while (*str)
	{
		if (*str == '*')
			return (1);
		str++;
	}
	return (0);
}

char *extract_directory_path(const char *pattern)
{
    char *last_slash;
    
    if (!pattern)
        return ft_strdup(".");
    last_slash = ft_strrchr(pattern, '/');
    if (!last_slash)
        return ft_strdup(".");
    if (last_slash == pattern)
        return ft_strdup("/");
    return ft_substr(pattern, 0, last_slash - pattern);
}

char *extract_filename_pattern(const char *pattern)
{
    char *last_slash;
    
    if (!pattern)
        return NULL;
    last_slash = ft_strrchr(pattern, '/');
    if (!last_slash)
        return ft_strdup(pattern);
    return ft_strdup(last_slash + 1);
}

int pattern_match(const char *pattern, const char *filename)
{
    int i;
    int j;
    
    if (!pattern || !filename)
        return 0;
    i = 0;
    j = 0;
    while (pattern[i] && filename[j])
    {
        if (pattern[i] == '*')
        {
            if (!pattern[i + 1])
                return 1;
            while (filename[j] && !pattern_match(&pattern[i + 1], &filename[j]))
                j++;
            i++;
        }
        else if (pattern[i] == filename[j])
        {
            i++;
            j++;
        }
        else
            return 0;
    }
    if (pattern[i] == '*' && !pattern[i + 1])
        return 1;
    return (!pattern[i] && !filename[j]);
}

int should_include_file(const char *filename, const char *pattern)
{
    if (!filename || !pattern)
        return 0;
    if (filename[0] == '.' && pattern[0] != '.')
        return 0;
    return pattern_match(pattern, filename);
}

// ========== WILDCARD EXPANSION ==========

char **collect_matching_files(const char *dir_path, const char *file_pattern)
{
    DIR *dir;
    struct dirent *entry;
    char **matches;
    int count;
    int capacity;
    
    dir = opendir(dir_path);
    if (!dir)
        return NULL;
    matches = malloc(sizeof(char *) * 64);
    if (!matches)
    {
        closedir(dir);
        return NULL;
    }
    count = 0;
    capacity = 64;
    while ((entry = readdir(dir)) != NULL)
    {
        if (should_include_file(entry->d_name, file_pattern))
        {
            if (count >= capacity - 1)
            {
                capacity *= 2;
                matches = ft_realloc(matches, sizeof(char *), capacity);
                if (!matches)
                {
                    closedir(dir);
                    return NULL;
                }
            }
            matches[count] = ft_strdup(entry->d_name);
            if (!matches[count])
            {
                ft_free_matrix(matches);
                closedir(dir);
                return NULL;
            }
            count++;
        }
    }
    closedir(dir);
    matches[count] = NULL;
    return matches;
}

void sort_string_array(char **array)
{
    int i;
    int j;
    char *temp;
    int len;
    
    if (!array)
        return;
    len = 0;
    while (array[len])
        len++;
    i = 0;
    while (i < len - 1)
    {
        j = 0;
        while (j < len - 1 - i)
        {
            if (ft_strcmp(array[j], array[j + 1]) > 0)
            {
                temp = array[j];
                array[j] = array[j + 1];
                array[j + 1] = temp;
            }
            j++;
        }
        i++;
    }
}

char **build_full_paths(const char *dir_path, char **filenames)
{
    char **full_paths;
    char *temp;
    int i;
    int count;
    
    if (!filenames)
        return NULL;
    count = 0;
    while (filenames[count])
        count++;
    full_paths = malloc(sizeof(char *) * (count + 1));
    if (!full_paths)
        return NULL;
    i = 0;
    while (i < count)
    {
        if (ft_strcmp(dir_path, ".") == 0)
            full_paths[i] = ft_strdup(filenames[i]);
        else
        {
            temp = ft_strjoin(dir_path, "/");
            if (!temp)
            {
                ft_free_matrix(full_paths);
                return NULL;
            }
            full_paths[i] = ft_strjoin(temp, filenames[i]);
            free(temp);
            if (!full_paths[i])
            {
                ft_free_matrix(full_paths);
                return NULL;
            }
        }
        i++;
    }
    full_paths[count] = NULL;
    return full_paths;
}

char **expand_wildcard_pattern(const char *pattern)
{
    char *dir_path;
    char *file_pattern;
    char **matches;
    char **full_paths;
    
    if (!pattern || !has_wildcard(pattern))
        return NULL;
    dir_path = extract_directory_path(pattern);
    file_pattern = extract_filename_pattern(pattern);
    if (!dir_path || !file_pattern)
    {
        free(dir_path);
        free(file_pattern);
        return NULL;
    }
    matches = collect_matching_files(dir_path, file_pattern);
    if (!matches || !matches[0])
    {
        free(dir_path);
        free(file_pattern);
        ft_free_matrix(matches);
        return NULL;
    }
    sort_string_array(matches);
    full_paths = build_full_paths(dir_path, matches);
    free(dir_path);
    free(file_pattern);
    ft_free_matrix(matches);
    return full_paths;
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
