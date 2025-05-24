/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elorente <elorente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 21:30:04 by elorente          #+#    #+#             */
/*   Updated: 2025/05/02 21:30:04 by elorente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Inc/minishell.h"

int	ft_pwd(t_env *env)
{
	char	cwd[PATH_MAX];
	char	*pwd_env;

	if (getcwd(cwd, PATH_MAX) != NULL)
	{
		ft_printf("%s\n", cwd);
		return (0);
	}
	else
	{
		pwd_env = get_env_value(env, "PWD");
		if (pwd_env)
		{
			ft_printf("%s\n", pwd_env);
			return (0);
		}
		perror("pwd");
		return (1);
	}
}
