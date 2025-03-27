#Variables
NAME	:=	libft.a
CC	:=	cc
CFLAGS	:=	-Wall -Wextra -Werror
ARFLAGS	:=	ar rcs
RM	:=	rm -rf 

# functions files
SRC		:=	ft_atoi.c \
ft_atol.c \
ft_bzero.c \
ft_calloc.c \
ft_count_dig_unsigned.c \
ft_count_digits.c \
ft_int_to_char.c \
ft_isalnum.c \
ft_isalpha.c \
ft_isascii.c \
ft_isdigit.c \
ft_islower.c \
ft_isprint.c \
ft_isspace.c \
ft_isupper.c \
ft_itoa.c \
ft_memchr.c \
ft_memcmp.c \
ft_memcpy.c \
ft_memmove.c \
ft_memrchr.c \
ft_memset.c \
ft_print_char.c \
ft_print_hex.c \
ft_print_int.c \
ft_print_ptr.c \
ft_print_str.c \
ft_printdec_unsig.c \
ft_printf.c \
ft_putchar_fd.c \
ft_putendl_fd.c \
ft_putnbr_fd.c \
ft_putnbr_unsigned_fd.c \
ft_putstr_fd.c \
ft_putstr_fd_int.c \
ft_split.c \
ft_strappend.c \
ft_strchar_count.c \
ft_strchr.c \
ft_strdup.c \
ft_striteri.c \
ft_strjoin.c \
ft_strlcat.c \
ft_strlcpy.c \
ft_strlen.c \
ft_strmapi.c \
ft_strncmp.c \
ft_strnstr.c \
ft_strrchr.c \
ft_strrev.c \
ft_strslice.c \
ft_strtrim.c \
ft_substr.c \
ft_swap_int.c \
ft_swap_void.c \
ft_tolower.c \
ft_toupper.c \
ft_word_count.c \
get_next_line.c \

OBJS		:=	$(SRC:.c=.o)


# Rules
all: $(NAME)
	@echo "\033[0;34mStatic library '$(NAME)' created.\n\033[0m"

$(NAME): $(OBJS)
	$(ARFLAGS) $(NAME) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $^ -o $@

clean:
	@$(RM) $(OBJS)
	@echo "\033[0;34mObject files '$(NAME)' removed.\n\033[0m"
	
fclean: clean 
	@$(RM) $(NAME)
	@echo "\033[0;34mStatic library '$(NAME)' removed.\n\033[0m"

re: fclean all 

# Phony declarations
.PHONY: all clean fclaen re 
