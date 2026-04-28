# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: fabiochaput <fabiochaput@student.42.fr>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/12/09 19:22:06 by lumarcuc          #+#    #+#              #
#    Updated: 2026/04/28 15:28:08 by fabiochaput      ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRCS = main.c \
       monitor_thread.c \
       parsing_init.c \
       utils.c \
	   heap_management.c \
	   actions.c

OBJS = $(SRCS:.c=.o)

CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread

NAME = main

RM = rm -f


all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

run: $(NAME)
	./$(NAME) 3 1 1 2 2 2 2 fifo

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)
	$(MAKE) -C fclean

re: fclean all

.PHONY: all clean fclean re run