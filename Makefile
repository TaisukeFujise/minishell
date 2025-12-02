# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/10/30 12:42:52 by tafujise          #+#    #+#              #
#    Updated: 2025/12/02 20:58:23 by tafujise         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME:=minishell

SRCDIR:=srcs
SRCS:=main.c\

HEADDIR:=includes
HEADERS:=$(HEADDIR)/minishell.h

LIBFT_DIR:=libft
LIBFT:=$(LIBFT_DIR)/libft.a


OBJDIR:=obj
OBJS:=$(addprefix $(OBJDIR)/,$(SRCS:%.c=%.o))

CC:=cc

CCFLAGS:=-Wall -Wextra -Werror -I$(HEADDIR) -I$(LIBFT_DIR)

DEBUG_FLAGS:=-g -O0

all:$(NAME)

$(LIBFT): FORCE
	make -C $(LIBFT_DIR)
FORCE:

$(NAME): $(OBJS) $(LIBFT)
	$(CC) -o $(NAME) $(OBJS) $(LIBFT)

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(HEADERS)
	@mkdir -p $(dir $@)
	$(CC) $(CCFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJDIR)
	make clean -C $(LIBFT_DIR)

fclean: clean
	rm -f $(NAME)
	make fclean -C $(LIBFT_DIR)

re: fclean all

debug: CCFLAGS += $(DEBUG_FLAGS)
debug:
	$(MAKE) fclean
	$(MAKE) -C $(LIBFT_DIR) CCFLAGS="$(CCFLAGS)" all
	$(MAKE) CCFLAGS="$(CCFLAGS)" all

.PHONY: all clean fclean re debug
