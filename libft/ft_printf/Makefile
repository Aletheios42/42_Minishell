# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: alepinto <alepinto@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/03/12 02:55:49 by alepinto          #+#    #+#              #
#    Updated: 2023/04/13 07:03:17 by alepinto         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

##### NAME #####
NAME = libftprintf.a

##### DIRECTORIES #####
OBJ_DIR = Objs

##### COMPILER #####
CC = gcc
CFLAGS = -Wall -Wextra -Werror

##### FILES #####
SRC =	ft_printf.c		\
		ft_typechecker.c	\
		ft_putnbr.c		\
		ft_putunbr.c		\
		ft_puthex.c		\
		ft_putchar.c		\
		ft_putstr.c		\
		ft_putptr.c		\

OBJ = $(addprefix $(OBJ_DIR)/, $(SRC:.c=.o))

##### RULES #####
all: $(NAME)

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJ_DIR) $(OBJ)
	ar -rcs $(NAME) $(OBJ)

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

clean:
	@rm -rf $(OBJ_DIR)
	@echo "Objetos de ft_printf eliminados"

fclean: clean
	@rm -f $(NAME)
	@echo "Biblioteca ft_printf eliminada"

re: fclean all

.PHONY: all clean fclean re
