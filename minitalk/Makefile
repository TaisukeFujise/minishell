NAME_SERVER = server
NAME_CLIENT = client

CC = cc
CFLAGS = -Wall -Wextra -Werror
INCLUDES = -I include -I ft_printf/include -I ft_printf/libft

FT_PRINTF_DIR = ft_printf
FT_PRINTF = $(FT_PRINTF_DIR)/libftprintf.a

SRC_SERVER = src/server.c
SRC_CLIENT = src/client.c
SRC_SERVER_BONUS = src/server_bonus.c
SRC_CLIENT_BONUS = src/client_bonus.c
SRC_UTILS = src/utils.c

OBJ_SERVER = $(SRC_SERVER:.c=.o) $(SRC_UTILS:.c=.o)
OBJ_CLIENT = $(SRC_CLIENT:.c=.o) $(SRC_UTILS:.c=.o)
OBJ_SERVER_BONUS = $(SRC_SERVER_BONUS:.c=.o) $(SRC_UTILS:.c=.o)
OBJ_CLIENT_BONUS = $(SRC_CLIENT_BONUS:.c=.o) $(SRC_UTILS:.c=.o)

HEADER = include/minitalk.h

MANDATORY_FLAG = .make_mandatory
BONUS_FLAG = .make_bonus

all: $(MANDATORY_FLAG)

bonus: $(BONUS_FLAG)

$(MANDATORY_FLAG): $(OBJ_SERVER) $(OBJ_CLIENT) $(FT_PRINTF)
	$(CC) $(CFLAGS) $(OBJ_SERVER) $(FT_PRINTF) -o $(NAME_SERVER)
	$(CC) $(CFLAGS) $(OBJ_CLIENT) $(FT_PRINTF) -o $(NAME_CLIENT)
	@touch $(MANDATORY_FLAG)
	@rm -f $(BONUS_FLAG)

$(BONUS_FLAG): $(OBJ_SERVER_BONUS) $(OBJ_CLIENT_BONUS) $(FT_PRINTF)
	$(CC) $(CFLAGS) $(OBJ_SERVER_BONUS) $(FT_PRINTF) -o $(NAME_SERVER)
	$(CC) $(CFLAGS) $(OBJ_CLIENT_BONUS) $(FT_PRINTF) -o $(NAME_CLIENT)
	@touch $(BONUS_FLAG)
	@rm -f $(MANDATORY_FLAG)

$(FT_PRINTF):
	make -C $(FT_PRINTF_DIR)

%.o: %.c $(HEADER)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	make -C $(FT_PRINTF_DIR) clean
	rm -f $(OBJ_SERVER) $(OBJ_CLIENT) $(OBJ_SERVER_BONUS) $(OBJ_CLIENT_BONUS)

fclean: clean
	make -C $(FT_PRINTF_DIR) fclean
	rm -f $(NAME_SERVER) $(NAME_CLIENT)
	rm -f $(MANDATORY_FLAG) $(BONUS_FLAG)

re: fclean all

.PHONY: all clean fclean re bonus