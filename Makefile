BIN = minishell
CC = gcc
CFLAGS = -Wall -Werror -Wextra
CFLAGS += -g3 -fsanitize=address

## Directories
SRC_DIR = ./Src/
LIBFT_DIR = ./libft/
OBJ_DIR = ./Obj/
INCLUDES_DIR = ./Inc/

## Files
SRC_FILES = main.c		\
			lexer.c		\
			parser.c	\
			executor.c	\
			print.c

SRC = $(addprefix $(SRC_DIR), $(SRC_FILES))
OBJ_FILES = $(SRC_FILES:.c=.o)
OBJ = $(addprefix $(OBJ_DIR), $(OBJ_FILES))

## Libft
LIBFT = $(LIBFT_DIR)libft.a

## Libraries and Includes
LIBS = -lreadline $(LIBFT)
INCLUDES = -I$(INCLUDES_DIR) -I$(LIBFT_DIR)

## Targets
all: $(OBJ_DIR) $(LIBFT) $(BIN)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(BIN): $(OBJ) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJ) $(LIBS) -o $(BIN)

$(OBJ_DIR)%.o: $(SRC_DIR)%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

clean:
	rm -rf $(OBJ_DIR)
	-$(MAKE) -C $(LIBFT_DIR) clean || true

fclean: clean
	rm -f $(BIN)
	-$(MAKE) -C $(LIBFT_DIR) fclean || true
re: fclean all

.PHONY: all clean fclean re
