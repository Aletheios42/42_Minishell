/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_v2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elorente <elorente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 23:38:02 by elorente          #+#    #+#             */
/*   Updated: 2025/05/29 23:38:02 by elorente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Inc/minishell.h"
#include "../Inc/signals.h"
#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

// ========== CLEANUP FUNCTIONS ==========

void	cleanup_resources(t_env *env)
{
	if (env)
		free_env_list(env);
	rl_clear_history();
}

// ========== INPUT PROCESSING ==========

int	process_tokens_and_tree(t_token *tokens, t_env **env, int exit_status)
{
	t_tree	*syntax_tree;
	int		result;

	syntax_tree = parser(tokens);
	if (!syntax_tree)
	{
		free_token_list(tokens);
		return (2);
	}
	result = execute_tree(syntax_tree, env, exit_status);
	free_syntax_tree(syntax_tree);
	return (result);
}
