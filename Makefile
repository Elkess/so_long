NAME = so_long
SRCS = so_long.c ft_strcmp.c ft_strrchr.c garbage_collector.c \
		get_next_line/get_next_line.c get_next_line/get_next_line_utils.c \
		ft_bzero.c

OBJS = $(SRCS:.c=.o)
CC = cc
CFLAGS =
LIBS = -Lmlx -lmlx -lXext -lX11 -lm

all: $(NAME)

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

%.o: %.c so_long.h
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -f $(OBJS)

fclean: clean
	@rm -f $(NAME)

re: fclean all

all: $(NAME)
