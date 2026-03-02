# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/02/16 23:09:01 by tafujise          #+#    #+#              #
#    Updated: 2026/02/16 23:15:31 by tafujise         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SUBDIRS = builtin \
          connection_pipe \
          disk \
          execute \
          hashmap \
          subshell

all:
	@set -e; \
	for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir; \
		./$$dir/$$(basename $$dir)_test; \
	done

clean:
	@set -e; \
	for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir clean; \
	done

fclean:
	@set -e; \
	for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir fclean; \
	done

re:
	@set -e; \
	for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir re; \
		./$$dir/$$(basename $$dir)_test; \
	done

.PHONY: all clean fclean re
