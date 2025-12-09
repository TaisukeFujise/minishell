SRC_DIR =	./src
SRC_LIST =	ft_printf.c ft_vfprintf.c \
			parse.c out.c emit.c \
			convert_char.c convert_str.c convert_int.c \
			convert_uint.c convert_hex.c convert_ptr.c \
			convert_float.c util.c
SRCS = $(addprefix $(SRC_DIR)/, $(SRC_LIST))
OBJS = $(SRCS:.c=.o)

LIBFT_DIR = ./libft
LIBFT = $(LIBFT_DIR)/libft.a

INCLUDE_DIR = ./include

NAME = libftprintf.a
CC = cc
CFLAGS = -Wall -Wextra -Werror -I$(INCLUDE_DIR) -I$(LIBFT_DIR) -fPIC
RM = rm -f
AR = ar rcs

all: $(NAME)

bonus: all

$(LIBFT):
	make -C $(LIBFT_DIR)

$(NAME): $(LIBFT) $(OBJS)
	cp $(LIBFT) $(NAME)
	$(AR) $(NAME) $(OBJS)

$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS)
	make clean -C $(LIBFT_DIR)

fclean: clean
	$(RM) $(NAME)
	make fclean -C $(LIBFT_DIR)

re: fclean all

.PHONY: all clean fclean re