# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: alepinto <alepinto@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/03/12 02:55:49 by alepinto          #+#    #+#              #
#    Updated: 2023/05/03 06:05:12 by alepinto         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Directorios
SRC_DIR = Src
OBJ_DIR = Objs
PRINTF_DIR = ft_printf

# Nombre de las bibliotecas
NAME = libft.a
PRINTF_LIB = $(PRINTF_DIR)/libftprintf.a

# Compilador y flags
CC = gcc
CFLAGS = -Wall -Wextra -Werror
INCLUDES = -I. -I$(PRINTF_DIR)

# Archivos fuente
SRC_FILES =	ft_isalpha.c		\
		ft_isdigit.c		\
		ft_isspace.c		\
		ft_isalnum.c		\
		ft_isascii.c		\
		ft_isprint.c		\
		ft_strlen.c		\
		ft_memset.c 		\
		ft_bzero.c		\
		ft_memcpy.c		\
		ft_memmove.c		\
		ft_strlcpy.c		\
		ft_strlcat.c		\
		ft_toupper.c		\
		ft_tolower.c		\
		ft_strchr.c		\
		ft_strrchr.c		\
		ft_strncmp.c		\
		ft_strcmp.c		\
		ft_memchr.c		\
		ft_memcmp.c		\
		ft_strtok.c		\
		ft_strarraystr.c	\
		ft_append_to_string_array.c	\
		ft_strnstr.c		\
		ft_free_matrix.c	\
		ft_atoi.c		\
		ft_calloc.c		\
		ft_realloc.c		\
		ft_strdup.c		\
		ft_substr.c		\
		ft_strjoin.c		\
		ft_strtrim.c		\
		ft_split.c		\
		ft_itoa.c		\
		ft_strmapi.c		\
		ft_striteri.c		\
		ft_putchar_fd.c		\
		ft_putstr_fd.c		\
		ft_putendl_fd.c		\
		ft_putnbr_fd.c		\
		get_next_line.c 	\
		get_next_line_utils.c	\
		ft_lstnew.c		\
		ft_lstadd_front.c	\
		ft_lstsize.c		\
		ft_lstlast.c		\
		ft_lstadd_back.c	\
		ft_lstdelone.c		\
		ft_lstclear.c		\
		ft_lstiter.c		\
		ft_lstmap.c			\

# Rutas completas
SRC = $(addprefix $(SRC_DIR)/, $(SRC_FILES))
OBJ = $(addprefix $(OBJ_DIR)/, $(SRC_FILES:.c=.o))

# Crear objetos en el directorio OBJ_DIR
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Regla principal
all: $(NAME)

# Compilar printf
$(PRINTF_LIB):
	@$(MAKE) -C $(PRINTF_DIR)

# Compilar libft combinada
$(NAME): $(OBJ_DIR) $(OBJ) $(PRINTF_LIB)
	@ar -rcs $(NAME) $(OBJ)
	@mkdir -p $(OBJ_DIR)/temp
	@cd $(OBJ_DIR)/temp && ar -x ../../$(PRINTF_LIB) && ar -rs ../../$(NAME) *.o
	@rm -rf $(OBJ_DIR)/temp
	@echo "Biblioteca $(NAME) creada correctamente"

# Crear directorios necesarios
$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

# Limpiar objetos
clean:
	@rm -rf $(OBJ_DIR)
	@$(MAKE) -C $(PRINTF_DIR) clean
	@echo "Objetos eliminados"

# Limpiar todo
fclean: clean
	@rm -f $(NAME)
	@$(MAKE) -C $(PRINTF_DIR) fclean
	@echo "Bibliotecas eliminadas"

# Reconstruir todo
re: fclean all

.PHONY: all clean fclean re
