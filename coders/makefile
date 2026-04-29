SRCS = main.c \
       monitor_thread.c \
       parsing_init.c \
       utils.c \
	   heap_management.c \
	   actions.c

OBJS = $(SRCS:.c=.o)

CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread

NAME = codexion

RM = rm -f


all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

run: $(NAME)
	./$(NAME) 8 1 856 2 2 2 256 edf

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)


re: fclean all

.PHONY: all clean fclean re run