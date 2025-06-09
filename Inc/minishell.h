/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elorente <elorente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 20:21:45 by alepinto          #+#    #+#             */
/*   Updated: 2025/06/09 16:18:58 by elorente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"

# include "structs.h"

// Local includes
# include "lexer.h"
# include "parser.h"
# include "executor.h"
# include "builtins.h"
# include "signals.h"

//main_v2.c

void	cleanup_resources(t_env *env);
int		process_tokens_and_tree(t_token *tokens, t_env **env, int exit_status);

# ifndef BUFF_SIZE
#  define BUFF_SIZE 1024
# endif

#endif
