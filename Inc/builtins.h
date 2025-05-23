#ifndef BUILTINS_H
#define BUILTINS_H

#include "structs.h"

// Built-in function prototypes
int ft_echo(char *args[]);
int ft_cd(char *args[], t_env **env);
int ft_pwd(void);
int ft_export(char *args[], t_env *env, t_env *secret);
int ft_unset(char *args[], t_env **env);
int ft_env(t_env *env);
int ft_exit(char *args[]);

// Built-in utility functions
int is_valid_identifier(const char *arg);
char *get_env_name(char *dest, const char *src);
int is_in_env(t_env *env, char *arg);
void env_add(const char *value, t_env *env);
int print_error(int error, const char *arg);
void env_update(const char *key, const char *value, t_env *env);
char *ft_strjoin_free(char *s1, const char *s2);

#endif
