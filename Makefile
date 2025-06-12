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
			main_v2.c		\
			signals/signals.c	\
			lexer/lexer.c	\
			lexer/lexer_utils.c	\
			lexer/lexer_utils2.c	\
			lexer/lexer_utils3.c	\
			lexer/lexer_utils4.c	\
			lexer/lexer_utils5.c	\
			lexer/lexer_utils6.c	\
			parser/parser.c	\
			parser/parser_v2.c	\
			parser/parser_v3.c	\
			parser/parser_v4.c	\
			execution/executor.c	\
			execution/executor_v2.c	\
			execution/executor_v3.c	\
			execution/executor_v4.c	\
			execution/executor_v5.c	\
			execution/executor_v6.c	\
			execution/executor_v7.c	\
			execution/executor_v8.c	\
			execution/expander.c	\
			execution/expander_v2.c	\
			execution/expander_v3.c	\
			execution/expander_v4.c	\
			execution/expander_v5.c	\
			execution/expander_v6.c	\
			execution/expander_v7.c	\
			execution/expander_v8.c	\
			execution/expander_v9.c	\
			execution/enviroment.c	\
			execution/enviroment_v2.c	\
			execution/enviroment_v3.c	\
			execution/enviroment_v4.c	\
			builtins/echo.c		\
			builtins/cd.c		\
			builtins/cd_v2.c		\
			builtins/pwd.c		\
			builtins/export_v1.c	\
			builtins/unset.c	\
			builtins/env.c		\
			builtins/exit.c		\
			debug/print_tokens.c \
			debug/print_tree.c \
			debug/print_tree_v2.c

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
	mkdir -p $(OBJ_DIR)/signals
	mkdir -p $(OBJ_DIR)/lexer
	mkdir -p $(OBJ_DIR)/parser
	mkdir -p $(OBJ_DIR)/execution
	mkdir -p $(OBJ_DIR)/builtins
	mkdir -p $(OBJ_DIR)/debug

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
