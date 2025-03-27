/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quit_quotes_token.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzolotar <mzolotar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 10:47:04 by mzolotar          #+#    #+#             */
/*   Updated: 2025/03/06 11:17:33 by mzolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief
 *
 * @param
 * @return
 */

bool is_outer_quote(char *content, int i, bool *in_single_quote,
                    bool *in_double_quote) {
  if (content[i] == '\'' || content[i] == '"') {
    if (!(*in_single_quote) && !(*in_double_quote)) {
      if (content[i] == '\'')
        *in_single_quote = true;
      else if (content[i] == '"')
        *in_double_quote = true;
      return true;
    } else if ((*in_single_quote && content[i] == '\'') ||
               (*in_double_quote && content[i] == '"')) {
      *in_single_quote = false;
      *in_double_quote = false;
      return true;
    }
  }
  return false;
}

/**
 * @brief
 *
 * @param
 * @return
 */

bool is_inner_quote(char *content, int i) {
  if (i > 0 && content[i + 1] != '\0') {
    if ((content[i] == '\'' || content[i] == '"') &&
        ft_isalnum(content[i - 1]) && ft_isalnum(content[i + 1])) {
      // Revisamos si hay un espacio antes o después
      int prev = i - 1;
      int next = i + 1;
      while (prev >= 0 && content[prev] == ' ')
        prev--;
      while (content[next] != '\0' && content[next] == ' ')
        next++;

      // Eliminamos solo si no hay espacios separando
      if (prev >= 0 && next < i + 1)
        return true;
    }
  }
  return false;
}

/**
 * @brief
 *
 * @param
 * @return
 */

void quit_quotes_token(t_tokens *current, t_metachars *meta) {
  if (!current || !current->content || current->content[0] == '\0')
    return;
  meta->i_current_index = 0;
  meta->j_new_index = 0;
  meta->in_single_quote = false;
  meta->in_double_quote = false;
  while (current->content[meta->i_current_index] != '\0') {
    if (is_outer_quote(current->content, meta->i_current_index,
                       &meta->in_single_quote, &meta->in_double_quote)) {
      meta->i_current_index++;
      continue;
    }
    if (is_inner_quote(current->content, meta->i_current_index)) {
      meta->i_current_index++;
      continue;
    }
    current->content[meta->j_new_index] =
        current->content[meta->i_current_index];
    meta->i_current_index++;
    meta->j_new_index++;
  }
  current->content[meta->j_new_index] = '\0';
}

/**
 * @brief
 *
 * @param
 * @return
 */

void check_and_quit_quotes_token(t_tokens *tokens, t_metachars *meta) {
  t_tokens *current;
  int i;
  bool is_quoted;

  current = tokens;
  while (current) {
    i = 0;
    meta->in_single_quote = false;
    meta->in_double_quote = false;
    is_quoted = false;
    while (current->content[i] != '\0') {
      toggle_quotes(current->content[i], &meta->in_single_quote,
                    &meta->in_double_quote);
      if (meta->in_single_quote || meta->in_double_quote)
        is_quoted = true;
      i++;
    }
    if (is_quoted && !meta->in_single_quote && !meta->in_double_quote) {
      quit_quotes_token(current, meta);
    }
    current = current->next;
  }
}
