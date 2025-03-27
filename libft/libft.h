/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzolotar <mzolotar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 08:48:36 by mzolotar          #+#    #+#             */
/*   Updated: 2025/03/07 14:03:32 by mzolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <limits.h>
# include <stdarg.h>
# include <stddef.h>
# include <stdlib.h>
# include <unistd.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 100
# endif

// mandatory functions:

int					ft_atoi(const char *nptr);
void				ft_bzero(void *s, size_t n);
void				*ft_calloc(size_t nmemb, size_t size);
int					ft_isalnum(int c);
int					ft_isalpha(int c);
int					ft_isascii(int c);
int					ft_isdigit(int c);
int					ft_isprint(int c);
char				*ft_itoa(int n);
void				*ft_memchr(const void *s, int c, size_t n);
int					ft_memcmp(const void *s1, const void *s2, size_t n);
void				*ft_memcpy(void *dest, const void *src, size_t n);
void				*ft_memmove(void *dest, const void *src, size_t n);
void				*ft_memset(void *ptr, int c, size_t n);
void				ft_putchar_fd(char c, int fd);
void				ft_putendl_fd(char *s, int fd);
void				ft_putnbr_fd(int n, int fd);
void				ft_putstr_fd(char *s, int fd);
int					ft_putstr_fd_int(char *s, int fd);
char				**ft_split(char const *s, char c);
char				*ft_strchr(const char *s, int c);
char				*ft_strdup(const char *s);
void				ft_striteri(char *s, void (*f)(unsigned int, char *));
char				*ft_strjoin(char const *s1, char const *s2);
size_t				ft_strlcat(char *dst, const char *src, size_t size);
size_t				ft_strlcpy(char *dst, const char *src, size_t size);
size_t				ft_strlen(const char *s);
char				*ft_strmapi(char const *s, char (*f)(unsigned int, char));
int					ft_strncmp(const char *s1, const char *s2, size_t n);
char				*ft_strnstr(const char *big, const char *little,
						size_t len);
char				*ft_strrchr(const char *s, int c);
char				*ft_strtrim(char const *s1, char const *set);
char				*ft_substr(char const *s, unsigned int start, size_t len);
int					ft_tolower(int c);
int					ft_toupper(int c);


// own funcions:
long int			ft_atol(const char *nptr);
int					ft_count_digits_unsigned(unsigned int number);
int					ft_count_digits(int number);
void				ft_int_to_char(int n, char *new_char, size_t size);
int					ft_islower(int c);
int					ft_isspace(int c);
int					ft_isupper(int c);
void				*ft_memrchr(const void *s, int c, size_t n);
void				ft_putnbr_unsigned_fd(unsigned int n, int fd);
void				ft_swap_int(int *a, int *b);
void				ft_swap_void(void *s1, void *s2);
char				*ft_strrev(char *str);
char				**ft_free(char **array);
size_t				ft_word_count(const char *s, char delimiter);
size_t				ft_strchar_count(const char *s, int c);
size_t				ft_count_words(char const *s, char c);

// printf:
int					ft_printf(char const *format, ...);

/*-----%c-----*/
int					ft_print_char(char c);

/*-----%s-----*/
int					ft_print_str(char *str);

/*-----%i + %d-----*/
int					ft_print_int(int number);

/*----%u----*/
int					ft_printdec_unsig(unsigned int number);

/*----%p----*/
int					ft_print_ptr(void *pointer);

/*----%x + %X----*/
int					ft_print_hex(unsigned int num, char format);

// Get_Next_Line:
char				*ft_strslice(const char *s, unsigned int start, size_t len);
char				*ft_strappend(char *line, char *temp);
char				*get_next_line(int fd);

#endif
