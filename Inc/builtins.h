/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elorente <elorente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 19:56:38 by elorente          #+#    #+#             */
/*   Updated: 2025/06/02 19:56:38 by elorente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

# include "structs.h"

// Built-in function prototypes
int		ft_echo(char *args[]);
int		ft_cd(char *args[], t_env **env);
int		ft_pwd(t_env *env);
int		ft_export(char *args[], t_env *env, t_env *secret);
int		ft_unset(char *args[], t_env **env);
int		ft_env(t_env *env);
int		ft_exit(char *args[]);

// Built-in utility functions
void	print_sorted_env(t_env *env);
int		process_export_argument(char *arg, t_env **env, t_env **secret);
int		update_env_dirs(t_env **env, const char *old_path);
int		change_to_path(const char *path, t_env **env);
int		handle_cd_home(t_env **env);
int		handle_cd_oldpwd(t_env **env);
void	env_update(const char *key, const char *value, t_env *env);
char	*ft_strjoin_free(char *s1, const char *s2);

#endif
