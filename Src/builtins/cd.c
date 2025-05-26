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

#include "../Inc/minishell.h"

int	update_env_dirs(t_env **env, const char *old_path)
{
	char	new_pwd[PATH_MAX];

	env_set_value(env, "OLDPWD", old_path);
	if (!getcwd(new_pwd, sizeof(new_pwd)))
		return (1);
	env_set_value(env, "PWD", new_pwd);
	return (0);
}

int	change_to_path(const char *path, t_env **env)
{
	char	old_pwd[PATH_MAX];

	if (!getcwd(old_pwd, sizeof(old_pwd)))
		return (1);
	if (chdir(path) != 0)
	{
		perror("cd");
		return (1);
	}
	return (update_env_dirs(env, old_pwd));
}

int	handle_cd_home(t_env **env)
{
	char	*path;

	path = get_env_value(*env, "HOME");
	if (!path)
	{
		ft_putendl_fd("minishell: cd: Home not set", 2);
		return (1);
	}
	return (change_to_path(path, env));
}

int	handle_cd_oldpwd(t_env **env)
{
	char	*path;

	path = get_env_value(*env, "OLDPWD");
	if (!path)
	{
		ft_putendl_fd("minishell: cd: OLDPWD not set", 2);
		return (1);
	}
	ft_putendl_fd(path, 1);
	return (change_to_path(path, env));
}

int	ft_cd(char *args[], t_env **env)
{
	if (!args[1])
		return (handle_cd_home(env));
	if (ft_strcmp(args[1], "-") == 0)
		return (handle_cd_oldpwd(env));
	return (change_to_path(args[1], env));
}
