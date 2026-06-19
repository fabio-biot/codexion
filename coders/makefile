SRCS = main.c \
       monitor_thread.c \
       parsing_init.c \
       parsing_norm.c \
       utils.c \
	   heap_management.c \
	   heap_manag_norm.c \
	   coder_routine.c \
	   utils_norm.c \
	   utils_norm_bis.c \
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
	./$(NAME) 8 45 2 2 2 2 2 edf

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)


re: fclean all

.PHONY: all clean fclean re run