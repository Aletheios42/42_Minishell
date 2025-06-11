/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elorente <elorente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 23:36:02 by alepinto          #+#    #+#             */
/*   Updated: 2025/06/11 21:02:39 by elorente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Inc/minishell.h"
#include "../Inc/signals.h"
#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

int	execute_input(char *input, t_env **env, int exit_status, int *should_exit)
{
	t_token	*tokens;
	int		result;

	if (!input || !*input)
	{
		return (exit_status);
	}
	tokens = lexer(input);

	if (!tokens)
	{
		ft_putendl_fd("minishell: syntax error", 2);
		return (2);
	}


	result = process_tokens_and_tree(tokens, env, exit_status);

	if (result >= 1000)
	{
		*should_exit = 1;
		if (result == 1000)
			return (0);
		if (result == 1001)
			return (1);
		if (result == 1255)
			return (255);
		return ((result - 1000) % 256);
	}
	return (result);
}


/*
int	execute_input(char *input, t_env **env, int exit_status, int *should_exit)
{
	t_token	*tokens;
	int		result;

	if (!input || !*input)
		return (exit_status);
	tokens = lexer(input);
	if (!tokens)
	{
		ft_putendl_fd("minishell: syntax error", 2);
		return (2);
	}
	result = process_tokens_and_tree(tokens, env, exit_status);
	if (result >= 1000)
	{
		*should_exit = 1;
		if (result == 1000)
			return (0);
		if (result == 1001)
			return (1);
		if (result == 1255)
			return (255);
		return ((result - 1000) % 256);
	}
	return (result);
}
*/

// ========== INPUT HANDLING ==========

char	*get_input_line(int is_command_mode, char **av)
{
	if (is_command_mode && av[2] == NULL)
	{
		ft_putendl_fd("minishell: -c requires an argument", 2);
		return (NULL);
	}
	if (is_command_mode)
		return (ft_strdup(av[2]));
	return (readline("minishell$ "));
}

int	should_continue_loop(char *input, int is_command_mode)
{
	if (!input)
		return (0);
	if (!*input)
		return (!is_command_mode);
	return (1);
}
int	run_shell_loop(t_env **env, int is_command_mode, char **av)
{
	char	*input;
	int		exit_status;
	int		should_exit;

	exit_status = 0;
	should_exit = 0;

	while (!should_exit)
	{
		input = get_input_line(is_command_mode, av);
		if (!should_continue_loop(input, is_command_mode))
		{
			if (!input)
				ft_putendl_fd("exit", 1);
			free(input);
			break ;
		}
		if (!is_command_mode)
			add_history(input);
		exit_status = execute_input(input, env, exit_status, &should_exit);
		free(input);
		if (is_command_mode)
			break ;
	}
	return (exit_status);
}


/*
int	run_shell_loop(t_env **env, int is_command_mode, char **av)
{
	char	*input;
	int		exit_status;
	int		should_exit;

	exit_status = 0;
	should_exit = 0;
	while (!should_exit)
	{
		input = get_input_line(is_command_mode, av);
		if (!should_continue_loop(input, is_command_mode))
		{
			if (!input)
				ft_putendl_fd("exit", 1);
			free(input);
			break ;
		}
		if (!is_command_mode)
			add_history(input);
		exit_status = execute_input(input, env, exit_status, &should_exit);
		free(input);
		if (is_command_mode)
			break ;
	}
	return (exit_status);
}
*/

// ========== MAIN LOOP ==========

int	main(int ac, char **av, char **envp)
{
	t_env	*env;
	int		is_command_mode;
	int		exit_status;

	env = env_from_array(envp);
	if (!env)
	{
		ft_putendl_fd("minishell: failed to initialize environment", 2);
		return (1);
	}
	is_command_mode = (ac >= 3 && ft_strcmp(av[1], "-c") == 0);
	if (!is_command_mode)
		setup_signals();
	exit_status = run_shell_loop(&env, is_command_mode, av);
	cleanup_resources(env);
	return (exit_status);
}
