/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elorente <elorente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 20:55:45 by elorente          #+#    #+#             */
/*   Updated: 2025/05/15 20:55:45 by elorente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Inc/minishell.h"

int	update_oldpwd(t_env *env)
{
	char	cwd[PATH_MAX];

	if (!getcwd(cwd, sizeof(cwd)))
		return (1);
	env_update("OLDPWD", cwd, env);
	return (0);
}

int	update_pwd(t_env *env)
{
	char	cwd[PATH_MAX];

	if (!getcwd(cwd, sizeof(cwd)))
		return (1);
	env_update("PWD", cwd, env);
	return (0);
}

int	change_to_path(const char *path, t_env *env)
{
	if (update_oldpwd(env) == 1)
		return (1);
	if (chdir(path) != 0)
	{
		perror("cd");
		return (1);
	}
	return (update_pwd(env));
}

int	ft_cd(char *args[], t_env *env[])
{
	char	*path;

	if (!args[1])
	{
		path = get_env_value(*env, "HOME");
		if (!path)
		{
			ft_putendl_fd("minishell: cd: Home not set", 2);
			return (1);
		}
		return (change_to_path(path, *env));
	}
	else if (ft_strcmp(args[1], "-") == 0)
	{
		path = get_env_value(*env, "OLDPWD");
		if (!path)
		{
			ft_putendl_fd("minishell: cd: OLDPWD not set", 2);
			return (1);
		}
		ft_putendl_fd(path, 1);
		return (change_to_path(path, *env));
	}
	else
		return (change_to_path(args[1], *env));
}
