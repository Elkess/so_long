NAME = so_long
SRCS = so_long.c helper_funcs.c parsing.c parsing_utils.c print.c \
		free_map.c get_next_line.c get_next_line_utils.c \

OBJS = $(SRCS:.c=.o)
CC = cc
CFLAGS = -Wall -Wextra -Werror -Imlx
MLX = mlx/libmlx.a
LDLIBS = -Lmlx -lmlx -lXext -lX11 -lm

all: $(NAME)

$(NAME): $(OBJS) $(MLX)
	$(CC) $(CFLAGS) $(OBJS) $(LDLIBS) -o $(NAME)

$(MLX):
	$(MAKE) -C mlx

%.o: %.c so_long.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -f $(OBJS)
	@$(MAKE) -C mlx clean

fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re