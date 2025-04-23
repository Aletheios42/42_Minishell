BIN = minishell
CC = gcc
CFLAGS = -Wall -Werror -Wextra

## SOURCE CODE
SRC_DIR = ./Src/
OBJ_DIR = ./Obj/

SRC_FILES = main.c
SRC = $(addprefix $(SRC_DIR), $(SRC_FILES))

OBJ_FILES = $(SRC_FILES:.c=.o)
OBJ = $(addprefix $(OBJ_DIR), $(OBJ_FILES))

## LINKER
LINKER=readline

##Includes
INCLUDES_DIR=./Inc 

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -l$(LINKER)  -o $(BIN)

$(OBJ_DIR)%.o: $(SRC_DIR)%.c
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -I$(INCLUDES_DIR)  -c $<  -o $@

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(BIN)

re: fclean all

.PHONY: all clean fclean re
