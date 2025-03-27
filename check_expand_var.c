/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_expand_var.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzolotar <mzolotar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 07:24:19 by mzolotar          #+#    #+#             */
/*   Updated: 2025/03/07 08:36:10 by mzolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief
 *
 * @param
 * @return
 */

// true -
// false -
bool is_valid_var(const char *str) {
  int i;

  i = 0;
  if (ft_strlen(str) == 1 && str[i] == '$')
    return (false);
  while (str[i]) {
    if (ft_strlen(str) >= 2) {
      if (str[i] == '$' && (str[i + 1] == '?'))
        return (true);
      if (str[i] == '$' && (str[i + 1] == '_' || (ft_isalpha(str[i + 1]))))
        return (true);
    }
    if (ft_strlen(str) >= 3) {
      if (str[i] == '$' && (str[i + 1] == '_' || ft_isalpha(str[i + 1])) &&
          (str[i + 2] == '_' || ft_isalpha(str[i + 2]) ||
           ft_isalnum(str[i + 2])))
        return (true);
    }
    i++;
  }
  return (false);
}

//(check_metacharacter(tokens->content, meta->var) == true) 	//true - $ con
//comillas (check_metacharacter(tokens->content, meta->var) == false) 	//false
//- $ sin comillas

bool check_extract_and_expand_var_token(t_tokens *tokens, t_metachars *meta) {
  t_tokens *current;

  current = tokens;
  while (current) {
    if (check_metacharacter(tokens->content, meta->var) ||
        !check_metacharacter(tokens->content,
                             meta->var)) // hay $ con o sin comillas
    {
      if (is_valid_var(tokens->content) == true) {
        // extract_expand_var(tokens->content);
        printf("hay posible_var en token\n");
        return true;
      }
    }

    current = current->next;
  }

  return false;
}
